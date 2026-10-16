#!/usr/bin/env python3
"""Verify editor recovery provenance against both untouched executable inputs."""
import hashlib
import json
from pathlib import Path
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from extract_original import coff, address_bytes
ROOT = Path(__file__).resolve().parents[1]
meta = json.loads((ROOT / 'analysis/jailed-recovery.json').read_text())
old = (ROOT / 'original/JAILED.EXE').read_bytes()
new = (ROOT / 'original-1.3b/JAILED.EXE').read_bytes()
assert hashlib.sha256(old).hexdigest() == meta['original10_sha256']
assert hashlib.sha256(new).hexdigest() == meta['original13_sha256']
old, symbols = coff(old)
new, _ = coff(new)
assert address_bytes(new, meta['version_address'], 5) == b'1.1b\0'
flip = meta['horizontal_flip']
assert any(s['name'] == '_draw_sprite_h_flip' and s['address'] == flip['old_address'] for s in symbols)
assert address_bytes(old, flip['old_address'], flip['identical_bytes']) == address_bytes(new, flip['new_address'], flip['identical_bytes'])
assert len(meta['functions']) == 13
for name, record in meta['functions'].items():
    source = (ROOT / 'analysis/jailed-1.3b' / (name + '.c')).read_bytes()
    assert hashlib.sha256(source).hexdigest() == record['sha256']
print('PASS both JAILED executable hashes, latest embedded version, 13 recovery snapshots and verified horizontal flip')
