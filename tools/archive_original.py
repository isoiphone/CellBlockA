#!/usr/bin/env python3
"""Package or verify the unchanged historical DOS files; never execute them."""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true', help='Verify the committed ZIP without changing it')
    parser.add_argument('--version', choices=('1.0', '1.3b'), default='1.0')
    args = parser.parse_args()
    suffix = '-1.3b' if args.version == '1.3b' else ''
    folder = ROOT / ('original' + suffix)
    prefix = 'CellBlockA-1.3b/' if suffix else 'CellBlockA-original/'
    archive_path = ROOT / 'archives' / (prefix.rstrip('/') + '.zip')
    manifest = json.loads((ROOT / ('analysis/original' + suffix + '-sha256.json')).read_text(encoding='utf-8'))
    for name, digest in manifest.items():
        if hashlib.sha256((folder / name).read_bytes()).hexdigest() != digest:
            raise RuntimeError('Original file changed: ' + name)
    if not args.check:
        archive_path.parent.mkdir(exist_ok=True)
        with zipfile.ZipFile(archive_path, 'w', compression=zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
            for name in sorted(manifest):
                # Fixed ZIP metadata, not a claim about historical file dates.
                info = zipfile.ZipInfo(prefix + name, date_time=(1980, 1, 1, 0, 0, 0))
                info.create_system = 3
                info.external_attr = 0o100644 << 16
                archive.writestr(info, (folder / name).read_bytes(),
                                 compress_type=zipfile.ZIP_DEFLATED, compresslevel=9)
    with zipfile.ZipFile(archive_path) as archive:
        expected = {prefix + name for name in manifest}
        if set(archive.namelist()) != expected or len(archive.namelist()) != len(expected):
            raise RuntimeError('Unexpected, missing, or duplicate archive entries')
        if archive.testzip() is not None:
            raise RuntimeError('Archive CRC check failed')
        for name, digest in manifest.items():
            if hashlib.sha256(archive.read(prefix + name)).hexdigest() != digest:
                raise RuntimeError('Archived file changed: ' + name)
    print(f'PASS {len(manifest)} unchanged {args.version} files in {archive_path.relative_to(ROOT)}')
    print('SHA-256:', hashlib.sha256(archive_path.read_bytes()).hexdigest())


if __name__ == '__main__':
    main()
