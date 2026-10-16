/* JAILED.EXE address: 000016f4 */

void _load_map(undefined4 param_1)

{
  undefined1 uVar1;
  byte bVar2;
  ushort uVar3;
  int iVar4;
  int local_10;
  int local_c;
  
  iVar4 = _pack_fopen(param_1,&LC8);
  if (iVar4 != 0) {
    _reset_map();
    for (local_c = 0; local_c < 0x14; local_c = local_c + 1) {
      for (local_10 = 0; local_10 < 0xd; local_10 = local_10 + 1) {
        uVar1 = _pack_getc(iVar4);
        (&_map)[local_10 * 2 + local_c * 0x1a] = uVar1;
      }
    }
    for (local_c = 0; local_c < 4; local_c = local_c + 1) {
      (&_start)[local_c * 4] = (&_start)[local_c * 4] | 0x7f;
      *(ushort *)(&_start + local_c * 4) = *(ushort *)(&_start + local_c * 4) | 0x3f80;
      bVar2 = _pack_getc(iVar4);
      (&_start)[local_c * 4] = (&_start)[local_c * 4] & 0x80;
      (&_start)[local_c * 4] = (&_start)[local_c * 4] | bVar2 & 0x7f;
      uVar3 = _pack_getc(iVar4);
      *(ushort *)(&_start + local_c * 4) = *(ushort *)(&_start + local_c * 4) & 0xc07f;
      *(ushort *)(&_start + local_c * 4) = *(ushort *)(&_start + local_c * 4) | (uVar3 & 0x7f) << 7;
      bVar2 = _pack_getc(iVar4);
      (&DAT_00079c85)[local_c * 4] = (&DAT_00079c85)[local_c * 4] & 0xbf;
      (&DAT_00079c85)[local_c * 4] = (&DAT_00079c85)[local_c * 4] | (bVar2 & 1) << 6;
    }
    _pack_fclose(iVar4);
  }
  return;
}

