/* JAILED.EXE address: 00001700 */

void _load_map(char *filename)

{
  byte *pbVar1;
  PACKFILE *f;
  int iVar2;
  int local_10;
  int local_c;
  
  f = _pack_fopen(filename,"rp");
  if (f != (PACKFILE *)0x0) {
    _reset_map();
    for (local_c = 0; local_c < 0x14; local_c = local_c + 1) {
      for (local_10 = 0; local_10 < 0xd; local_10 = local_10 + 1) {
        iVar2 = _pack_getc(f);
        _map[local_c][local_10].kind = (uchar)iVar2;
      }
    }
    for (local_c = 0; local_c < 4; local_c = local_c + 1) {
      *(byte *)&_start[local_c].packed = (byte)_start[local_c].packed | 0x7f;
      _start[local_c].packed = _start[local_c].packed | 0x3f80;
      iVar2 = _pack_getc(f);
      *(byte *)&_start[local_c].packed = (byte)_start[local_c].packed & 0x80;
      *(byte *)&_start[local_c].packed = (byte)_start[local_c].packed | (byte)iVar2 & 0x7f;
      iVar2 = _pack_getc(f);
      _start[local_c].packed = _start[local_c].packed & 0xc07f;
      _start[local_c].packed = _start[local_c].packed | ((ushort)iVar2 & 0x7f) << 7;
      iVar2 = _pack_getc(f);
      pbVar1 = (byte *)((int)&_start[local_c].packed + 1);
      *pbVar1 = *pbVar1 & 0xbf;
      pbVar1 = (byte *)((int)&_start[local_c].packed + 1);
      *pbVar1 = *pbVar1 | ((byte)iVar2 & 1) << 6;
    }
    _pack_fclose(f);
  }
  return;
}

