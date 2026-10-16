/* Original 1.3b CELL.EXE address: 00001e3c */

void _start_music(void)

{
  int iVar1;
  
  iVar1 = _is_mod_playing();
  if ((iVar1 == 0) && (_nosound == 0)) {
    if (_music == (MOD *)0x0) {
      _rand_song();
    }
    else if (_BLOODBATH == 0) {
      _play_mod(_music,-1);
    }
  }
  return;
}

