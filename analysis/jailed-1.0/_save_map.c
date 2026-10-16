/* JAILED.EXE address: 00002950 */

void _save_map(char *param_1)

{
  int iVar1;
  size_t sVar2;
  undefined4 uVar3;
  int local_14;
  int local_10;
  int local_c;
  
  local_14 = 0;
  for (local_c = 0; local_c < 4; local_c = local_c + 1) {
    if (((char)((&_start)[local_c * 4] * '\x02') >> 1 != -1) &&
       ((short)(*(short *)(&_start + local_c * 4) << 2) >> 9 != -1)) {
      local_14 = local_14 + 1;
    }
  }
  if (local_14 < 2) {
    _alert("Could not save:","Not enough start points!",0,&LC25,0,0,0);
  }
  else {
    iVar1 = _pack_fopen(param_1,&LC28);
    if (iVar1 != 0) {
      sVar2 = _strlen(param_1);
      param_1[sVar2 - 3] = 'p';
      sVar2 = _strlen(param_1);
      param_1[sVar2 - 2] = 'i';
      sVar2 = _strlen(param_1);
      param_1[sVar2 - 1] = 'c';
      uVar3 = _mappic(&__current_pallete);
      _save_pcx(param_1,uVar3);
      for (local_c = 0; local_c < 0x14; local_c = local_c + 1) {
        for (local_10 = 0; local_10 < 0xd; local_10 = local_10 + 1) {
          _pack_putc((&_map)[local_10 * 2 + local_c * 0x1a],iVar1);
        }
      }
      for (local_c = 0; local_c < 4; local_c = local_c + 1) {
        _pack_putc((int)((char)((&_start)[local_c * 4] * '\x02') >> 1),iVar1);
        _pack_putc((int)((short)(*(short *)(&_start + local_c * 4) << 2) >> 9),iVar1);
        _pack_putc((byte)(&DAT_00079c85)[local_c * 4] >> 6 & 1,iVar1);
      }
      _pack_fclose(iVar1);
    }
  }
  return;
}

