/* JAILED.EXE address: 000015e4 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _reset_map(void)

{
  int local_c;
  int local_8;
  
  for (local_8 = 0; local_8 < 0x14; local_8 = local_8 + 1) {
    for (local_c = 0; local_c < 0xd; local_c = local_c + 1) {
      (&_map)[local_c * 2 + local_8 * 0x1a] = 0;
      (&DAT_00079a69)[local_c * 2 + local_8 * 0x1a] = 0;
    }
  }
  __start = CONCAT11((char)(__start >> 8),1);
  __start = __start & 0x807f | 0x100;
  _DAT_00079c88 = CONCAT11((char)(_DAT_00079c88 >> 8),0x12);
  _DAT_00079c88 = _DAT_00079c88 & 0xc07f;
  _DAT_00079c88 = _DAT_00079c88 | 0x4100;
  _DAT_00079c8c = _DAT_00079c8c & 0xbfff | 0x3fff;
  _DAT_00079c90 = _DAT_00079c90 & 0xbfff | 0x3fff;
  return;
}

