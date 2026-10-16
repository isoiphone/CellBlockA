#!/usr/bin/env python3
"""Losslessly unpack Cell Block A's DJGPP COFF and Allegro packfiles."""
import hashlib
import json
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def unpack(data):
    if data[:4] == b'slh.':
        return data[4:]
    if data[:4] != b'slh!':
        raise ValueError('Not an Allegro packfile')
    return lzss(data[4:])


def lzss(data):
    ring = bytearray(4096)
    pos, write, flags = 0, 4096 - 18, 0
    result = bytearray()
    while pos < len(data):
        flags >>= 1
        if not flags & 256:
            flags = data[pos] | 0xff00
            pos += 1
            if pos == len(data):
                break
        if flags & 1:
            values = [data[pos]]
            pos += 1
        else:
            if pos + 2 > len(data):
                raise ValueError('Truncated LZSS match')
            lo, hi = data[pos:pos + 2]
            pos += 2
            read = lo | ((hi & 0xf0) << 4)
            # The match may overlap the ring write position.
            for k in range((hi & 15) + 3):
                c = ring[(read + k) & 4095]
                result.append(c)
                ring[write] = c
                write = (write + 1) & 4095
            continue
        for c in values:
            result.append(c)
            ring[write] = c
            write = (write + 1) & 4095
    return bytes(result)


def archive(data, destination):
    assert data[:4] == b'ALL.'
    count, = struct.unpack_from('>I', data, 4)
    pos = 8
    entries, props = [], {}
    destination.mkdir(parents=True, exist_ok=True)
    while len(entries) < count:
        kind = data[pos:pos + 4].decode('ascii')
        pos += 4
        if kind == 'prop':
            key = data[pos:pos + 4].decode('ascii')
            size, = struct.unpack_from('>I', data, pos + 4)
            props[key] = data[pos + 8:pos + 8 + size].decode('latin1')
            pos += 8 + size
            continue
        packed, size = struct.unpack_from('>ii', data, pos)
        pos += 8
        chunk = data[pos:pos + packed]
        pos += packed
        if size < 0:
            chunk = lzss(chunk)
        assert len(chunk) == abs(size), (kind, size, len(chunk))
        name = f'{len(entries):03d}.bin'
        (destination / name).write_bytes(chunk)
        entry = dict(index=len(entries), type=kind, size=len(chunk), file=name, properties=props)
        if kind in ('BMP ', 'RLE '):
            entry.update(zip(('depth', 'width', 'height'), struct.unpack_from('>HHH', chunk)))
        entries.append(entry)
        props = {}
    assert pos == len(data), (pos, len(data))
    (destination / 'index.json').write_text(json.dumps(entries, indent=2) + '\n')
    return entries


def coff(data):
    pages, = struct.unpack_from('<H', data, 4)
    last, = struct.unpack_from('<H', data, 2)
    stub = pages * 512 - (512 - last if last else 0)
    data = data[stub:]
    magic, count, timestamp, table, total, opt, flags = struct.unpack_from('<HHIIIHH', data)
    assert magic == 0x14c
    strings = data[table + total * 18:]
    symbols = []
    i = 0
    while i < total:
        raw, value, section, typ, storage, auxiliary = struct.unpack_from('<8sIhHBB', data, table + i * 18)
        if raw[:4] == b'\0' * 4:
            raw = strings[struct.unpack_from('<I', raw, 4)[0]:]
        name = raw.split(b'\0')[0].decode('latin1')
        symbols.append(dict(index=i, name=name, address=value, section=section, type=typ, storage=storage))
        i += 1 + auxiliary
    return data, symbols


def address_bytes(data, address, size):
    """Read a virtual address using COFF section offsets, including stripped files."""
    _, count, _, _, _, optional, _ = struct.unpack_from('<HHIIIHH', data)
    for i in range(count):
        fields = struct.unpack_from('<8sIIIIIIHHI', data, 20 + optional + i * 40)
        base, length, offset = fields[2:5]
        if offset and base <= address and address + size <= base + length:
            return data[offset + address - base:offset + address - base + size]
    raise ValueError(f'Unmapped COFF address: {address:#x} ({size} bytes)')


def main():
    out = ROOT / 'analysis'
    out.mkdir(exist_ok=True)
    manifest = {}
    for p in sorted((ROOT / 'original').rglob('*')):
        if p.is_file():
            manifest[p.relative_to(ROOT / 'original').as_posix()] = hashlib.sha256(p.read_bytes()).hexdigest()
    manifest_path = out / 'original-sha256.json'
    if manifest_path.exists():
        assert json.loads(manifest_path.read_text()) == manifest, 'Original files changed since first extraction'
    else:
        manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
    current = ROOT / 'original-1.3b'
    expected = json.loads((out / 'original-1.3b-sha256.json').read_text())
    actual = {p.relative_to(current).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
              for p in current.rglob('*') if p.is_file()}
    if expected != actual:
        raise RuntimeError('Original 1.3b inputs changed')
    link = out / 'original'
    if link.is_symlink():
        link.unlink()
    if not link.exists():
        # A relative symlink keeps an ordinary Mac/Linux developer build relocatable.
        # Windows builds use their staged resources and do not need this link.
        import os
        if os.name != 'nt':
            link.symlink_to('../original-1.3b', target_is_directory=True)
    for name in ('CELL', 'JAILED'):
        data, symbols = coff((current / (name + '.EXE')).read_bytes())
        (out / (name + '.coff')).write_bytes(data)
        # LLVM objdump treats DJGPP's absolute symbols as section-relative.
        # This separate copy is ONLY for LLVM objdump. Ghidra correctly reads
        # the unmodified executable COFF and must use NAME.coff instead.
        normalized = bytearray(data)
        _, sections, _, table, _, opt, _ = struct.unpack_from('<HHIIIHH', data)
        bases = {i + 1: struct.unpack_from('<I', data, 20 + opt + i * 40 + 12)[0]
                 for i in range(sections)}
        for s in symbols:
            if s['section'] in bases:
                struct.pack_into('<I', normalized, table + s['index'] * 18 + 8,
                                 s['address'] - bases[s['section']])
        (out / (name + '-normalized.coff')).write_bytes(normalized)
        (out / (name + '-symbols.json')).write_text(json.dumps(symbols, indent=2) + '\n')
        (out / (name + '-globals.tsv')).write_text(''.join(f"{s['address']:08x}\t{s['section']}\t{s['name']}\n" for s in symbols if s['storage'] == 2))
    for name in ('GAME', 'MENU'):
        data = unpack((current / (name + '.DAT')).read_bytes())
        entries = archive(data, out / 'assets' / name)
        print(name, len(entries), 'objects')
    dest = out / 'maps'
    dest.mkdir(exist_ok=True)
    for p in (current / 'MAPS').glob('*.CEL'):
        data = unpack(p.read_bytes())
        (dest / (p.stem + '.bin')).write_bytes(data)
        print(p.name, len(data), 'bytes')


if __name__ == '__main__':
    main()
