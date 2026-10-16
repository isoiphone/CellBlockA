#!/usr/bin/env python3
"""Verify the native source archive in isolation, without excluded local files."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tarfile
import tempfile
from release_files import ROOT


def main():
    subprocess.run([sys.executable, str(ROOT / 'tools/export_source.py')], check=True)
    build = ROOT / 'build'
    build.mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='source-check-', dir=build) as temporary:
        destination = Path(temporary)
        with tarfile.open(ROOT / 'dist/CellBlockA-source.tar.gz') as archive:
            # Only regular, relative files from our freshly generated archive.
            for member in archive.getmembers():
                parts = Path(member.name).parts
                if (not member.isfile() or not parts or parts[0] != 'CellBlockA'
                        or '..' in parts or Path(member.name).is_absolute()):
                    raise RuntimeError('Unexpected source archive entry: ' + member.name)
                target = destination / member.name
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(archive.extractfile(member).read())
                target.chmod(member.mode & 0o777)
        checkout = destination / 'CellBlockA'
        manifest = json.loads((checkout / 'SOURCE_MANIFEST.json').read_text(encoding='utf-8'))
        actual = {p.relative_to(checkout).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
                  for p in checkout.rglob('*')
                  if p.is_file() and p.name != 'SOURCE_MANIFEST.json'}
        if manifest != actual:
            raise RuntimeError('Source archive does not match its manifest')
        subprocess.run([sys.executable, str(checkout / 'tools/check_repo.py')],
                       cwd=checkout, check=True)
    print('PASS isolated source archive, complete manifest, and documentation checks')


if __name__ == '__main__':
    main()
