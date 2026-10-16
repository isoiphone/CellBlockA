/* Original CELL.EXE address: 00001ce0 */

void _prev_song(void)

{
  if ((0 < _num_mods) && (_nosound == 0)) {
    _stop_mod();
    while( true ) {
      _cursong = _cursong->next;
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

