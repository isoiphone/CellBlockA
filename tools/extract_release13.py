#!/usr/bin/env python3
"""Verify (or extract) the archived 1.3b installer as ZIP data, without executing it."""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--extract', action='store_true', help='Restore original-1.3b from the verified installer')
    args = parser.parse_args()
    recovery = json.loads((ROOT / 'analysis/recovery-1.3b.json').read_text())
    installer = ROOT / recovery['installer']
    if not installer.exists():
        raise SystemExit('This provenance check requires the full repository, including www/downloads/cellin13.exe.')
    if hashlib.sha256(installer.read_bytes()).hexdigest() != recovery['installer_sha256']:
        raise RuntimeError('The archived installer hash changed')
    expected = json.loads((ROOT / 'analysis/original-1.3b-sha256.json').read_text())
    entries = {}
    with zipfile.ZipFile(installer) as archive:
        for member in archive.infolist():
            if member.is_dir():
                continue
            name = member.filename.replace('\\', '/').upper()
            path = Path(name)
            if path.is_absolute() or '..' in path.parts or name in entries:
                raise RuntimeError('Unsafe or duplicate installer member: ' + name)
            entries[name] = archive.read(member)
    if {name: hashlib.sha256(data).hexdigest() for name, data in entries.items()} != expected:
        raise RuntimeError('Installer contents do not match the recorded 1.3b release')
    for name, data in entries.items():
        target = ROOT / 'original-1.3b' / name
        if args.extract:
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(data)
        if target.read_bytes() != data:
            raise RuntimeError('Extracted 1.3b file differs from installer: ' + name)
    print(f'PASS installer SHA-256, ZIP CRCs, and all {len(entries)} original 1.3b files')


if __name__ == '__main__':
    main()
