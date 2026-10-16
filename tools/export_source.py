#!/usr/bin/env python3
"""Export the explicit native-game source set; never traverse unrelated folders."""
import argparse
import hashlib
import io
import json
from pathlib import Path
import re
import tarfile

ROOT = Path(__file__).resolve().parents[1]
FILES = (
    'README.md', 'LICENSE', 'ASSET_LICENSE.md', 'THIRD_PARTY_NOTICES.md', 'CONTRIBUTING.md', 'CHANGELOG.md',
    '.gitignore', '.gitattributes',
    '.github/workflows/windows.yml', '.github/workflows/native.yml', '.github/workflows/release.yml',
    '.github/ISSUE_TEMPLATE/bug_report.yml',
    'analysis/types.h', 'analysis/original-sha256.json',
    'analysis/types-1.3b.h', 'analysis/original-1.3b-sha256.json', 'analysis/recovery-1.3b.json',
    'analysis/symbols-1.3b.tsv', 'analysis/jailed-symbols-1.3b.tsv',
    'analysis/jailed-types-1.3b.h', 'analysis/jailed-recovery.json',
    'vendor/JGMOD-main/readme.txt', 'vendor/JGMOD-main/README.md',
    'docs/BUILDING.md', 'docs/WINDOWS.md', 'docs/PLAYING.md', 'docs/FIDELITY.md',
    'docs/JAILED.md', 'docs/RECOVERY.md', 'docs/RELEASING.md', 'docs/images/gameplay.png',
)
TREES = ('src', 'tests', 'analysis/typed', 'analysis/typed-1.3b', 'analysis/jailed-1.3b', 'analysis/jailed-1.0', 'vendor/JGMOD-main/src')


def export_bytes(path):
    data = path.read_bytes()
    if path == ROOT / 'README.md':
        # The repository README may link to separately maintained website work.
        # Adapt only the exported copy; never inspect or change that work.
        paragraphs = data.decode('utf-8').split('\n\n')
        exported = []
        noted_extras = False
        for paragraph in paragraphs:
            if re.search(r'\]\((?:www|archives)/', paragraph):
                if not noted_extras:
                    exported.append('The historical websites and convenience DOS ZIP are included '
                                    'in the full repository but not included in this native-game '
                                    'source archive. The unpacked original game files remain '
                                    'included under `original-1.3b/` (current build inputs) '
                                    'and `original/` (1.0 research reference).')
                    noted_extras = True
            else:
                exported.append(paragraph)
        data = '\n\n'.join(exported).encode('utf-8')
    return data


def source_files():
    files = {ROOT / name for name in FILES}
    for folder in TREES:
        files.update(p for p in (ROOT / folder).rglob('*')
                     if p.is_file() and '__pycache__' not in p.parts and p.name != '.DS_Store')
    files.update(p for p in (ROOT / 'tools').iterdir()
                 if p.is_file() and p.suffix in ('.py', '.sh', '.java'))
    original = json.loads((ROOT / 'analysis/original-sha256.json').read_text())
    files.update(ROOT / 'original' / name for name in original)
    current = json.loads((ROOT / 'analysis/original-1.3b-sha256.json').read_text())
    files.update(ROOT / 'original-1.3b' / name for name in current)
    for p in files:
        if not p.is_file() or p.is_symlink():
            raise RuntimeError('Missing source input or unexpected symlink: ' + str(p))
    return sorted(files, key=lambda p: p.relative_to(ROOT).as_posix())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--list', action='store_true', help='Print only the native source file list')
    args = parser.parse_args()
    files = source_files()
    if args.list:
        print('\n'.join(p.relative_to(ROOT).as_posix() for p in files))
        return
    original = json.loads((ROOT / 'analysis/original-sha256.json').read_text())
    for name, digest in original.items():
        if hashlib.sha256((ROOT / 'original' / name).read_bytes()).hexdigest() != digest:
            raise RuntimeError('Original file changed: ' + name)
    current = json.loads((ROOT / 'analysis/original-1.3b-sha256.json').read_text())
    for name, digest in current.items():
        if hashlib.sha256((ROOT / 'original-1.3b' / name).read_bytes()).hexdigest() != digest:
            raise RuntimeError('Original 1.3b file changed: ' + name)
    manifest = {}
    archive = ROOT / 'dist/CellBlockA-source.tar.gz'
    archive.parent.mkdir(exist_ok=True)
    with tarfile.open(archive, 'w:gz') as tar:
        for p in files:
            name = p.relative_to(ROOT).as_posix()
            data = export_bytes(p)
            manifest[name] = hashlib.sha256(data).hexdigest()
            info = tar.gettarinfo(p, arcname='CellBlockA/' + name)
            info.size = len(data)
            # Do not leak local account names through archive ownership fields.
            info.uid = info.gid = 0
            info.uname = info.gname = ''
            tar.addfile(info, io.BytesIO(data))
        data = (json.dumps(manifest, indent=2) + '\n').encode('utf-8')
        info = tarfile.TarInfo('CellBlockA/SOURCE_MANIFEST.json')
        info.size = len(data)
        tar.addfile(info, io.BytesIO(data))
    print(f'Exported {len(files)} native source files: {archive}')


if __name__ == '__main__':
    main()
