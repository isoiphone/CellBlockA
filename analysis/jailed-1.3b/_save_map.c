/* JAILED.EXE address: 0000295c */

void _save_map(char *filename)

{
  PACKFILE *f;
  size_t sVar1;
  BITMAP *bmp;
  RGB *pal;
  int local_14;
  int local_10;
  int local_c;
  
  local_14 = 0;
  for (local_c = 0; local_c < 4; local_c = local_c + 1) {
    if (((char)((char)_start[local_c].packed * '\x02') >> 1 != -1) &&
       ((short)(_start[local_c].packed << 2) >> 9 != -1)) {
      local_14 = local_14 + 1;
    }
  }
  if (local_14 < 2) {
    _alert("Could not save:","Not enough start points!",0,&DAT_0000292a,0,0,0);
  }
  else {
    f = _pack_fopen(filename,"wp");
    if (f != (PACKFILE *)0x0) {
      sVar1 = _strlen(filename);
      filename[sVar1 - 3] = 'p';
      sVar1 = _strlen(filename);
      filename[sVar1 - 2] = 'c';
      sVar1 = _strlen(filename);
      filename[sVar1 - 1] = 'x';
      pal = (RGB *)&__current_pallete;
      bmp = _mappic();
      _save_pcx(filename,bmp,pal);
      for (local_c = 0; local_c < 0x14; local_c = local_c + 1) {
        for (local_10 = 0; local_10 < 0xd; local_10 = local_10 + 1) {
          _pack_putc(_map[local_c][local_10].kind,f);
        }
      }
      for (local_c = 0; local_c < 4; local_c = local_c + 1) {
        _pack_putc((int)((char)((char)_start[local_c].packed * '\x02') >> 1),f);
        _pack_putc((int)((short)(_start[local_c].packed << 2) >> 9),f);
        _pack_putc(*(byte *)((int)&_start[local_c].packed + 1) >> 6 & 1,f);
      }
      _pack_fclose(f);
    }
  }
  return;
}

