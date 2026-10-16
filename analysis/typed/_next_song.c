/* Original CELL.EXE address: 00001c60 */

void _next_song(void)

{
  if ((0 < _num_mods) && (_nosound == 0)) {
    _stop_mod();
    while( true ) {
      _cursong = *(FileNode **)_cursong[1].name;
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

