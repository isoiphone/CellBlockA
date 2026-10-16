/* Original 1.3b CELL.EXE address: 00001d6c */

void _rand_song(void)

{
  int iVar1;
  int local_c;
  
  if ((0 < _num_mods) && (_nosound == 0)) {
    iVar1 = _rand();
    for (local_c = 0; local_c < iVar1 % (_num_mods * 2); local_c = local_c + 1) {
      _cursong = (ModNode *)_cursong->next;
    }
    _stop_mod();
    while( true ) {
      _cursong = (ModNode *)_cursong->next;
      _destroy_mod(_music);
      _music = _load_mod(_cursong->name);
      if (_music != (MOD *)0x0) break;
      _music = (MOD *)0x0;
    }
    if (_BLOODBATH == 0) {
      _play_mod(_music,-1);
    }
  }
  return;
}

