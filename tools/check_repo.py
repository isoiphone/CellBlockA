#!/usr/bin/env python3
"""Check native release inputs and local documentation links without scanning the website."""
import ast
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
from urllib.parse import unquote, urlsplit
from export_source import ROOT, source_files


def main():
    files = source_files()
    original = json.loads((ROOT / 'analysis/original-sha256.json').read_text())
    actual = {p.relative_to(ROOT / 'original').as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
              for p in (ROOT / 'original').rglob('*') if p.is_file()}
    if original != actual:
        raise RuntimeError('Original input hashes or file set changed')
    current = json.loads((ROOT / 'analysis/original-1.3b-sha256.json').read_text())
    actual_current = {p.relative_to(ROOT / 'original-1.3b').as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
                      for p in (ROOT / 'original-1.3b').rglob('*') if p.is_file()}
    if current != actual_current:
        raise RuntimeError('Original 1.3b input hashes or file set changed')
    from extract_original import coff, address_bytes
    recovery = json.loads((ROOT / 'analysis/recovery-1.3b.json').read_text())
    binary, _ = coff((ROOT / 'original-1.3b/CELL.EXE').read_bytes())
    if address_bytes(binary, recovery['version_string_address'], 5) != b'1.3b\0':
        raise RuntimeError('Native reconstruction must use the 1.3b executable')
    failures = []
    total = 0
    for p in files:
        rel = p.relative_to(ROOT).as_posix()
        total += p.stat().st_size
        if p.stat().st_size > 50 * 1024 * 1024:
            failures.append('Unexpectedly large source file: ' + rel)
        if p.suffix == '.py':
            ast.parse(p.read_text(encoding='utf-8'), filename=rel)
        if p.suffix == '.md' and not rel.startswith('vendor/'):
            text = p.read_text(encoding='utf-8')
            for target in re.findall(r'!?\[[^\]]*\]\(([^)]+)\)', text):
                target = target.strip('<>')
                parsed = urlsplit(target)
                if parsed.scheme or not parsed.path:
                    continue
                path = Path(os.path.abspath(p.parent / unquote(parsed.path)))
                if path == ROOT / 'www' or ROOT / 'www' in path.parents:
                    continue  # Separate website work is outside this check's scope.
                if not path.exists():
                    failures.append(f'{rel}: broken local link {target}')
        if p.suffix in ('.py', '.sh', '.cpp', '.h', '.md') and not rel.startswith('vendor/'):
            text = p.read_text(encoding='utf-8')
            personal_path = '/' + r'Users/[^/]+/|' + '/' + r'home/[^/]+/'
            if re.search(personal_path, text):
                failures.append('Personal absolute path in ' + rel)
    if 'MIT License' not in (ROOT / 'LICENSE').read_text():
        failures.append('Missing MIT source license')
    if failures:
        raise SystemExit('\n'.join(failures))
    subprocess.run([sys.executable, str(ROOT / 'tests/test_release_tools.py')], check=True)
    subprocess.run([sys.executable, str(ROOT / 'tests/test_jailed_recovery.py')], check=True)
    print(f'PASS {len(original)} archival 1.0 and {len(current)} original 1.3b hashes; verified 1.3b version; {len(files)} source inputs '
          f'({total / 1024 / 1024:.1f} MiB); Python syntax; native documentation links')


if __name__ == '__main__':
    main()
