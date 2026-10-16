/* Original CELL.EXE address: 0000b0a4 */

void _game_tick(void)

{
  int iVar1;
  int color;
  int type;
  
  _game_time = _game_time + 1.0;
  _translate_player(0);
  _translate_player(1);
  if ((0 < _game_config.maxtime) &&
     (iVar1 = _clock(), _game_config.maxtime < (iVar1 - _match_start) / 0x5b)) {
    if (_player[0].health < _player[1].health) {
      _damage_player(0,_player[0].health,_player[0].x + 8,_player[0].height / 2 + _player[0].y,5);
    }
    else if (_player[1].health < _player[0].health) {
      _damage_player(1,_player[1].health,_player[1].x + 8,_player[1].height / 2 + _player[1].y,5);
    }
    else {
      _damage_player(0,_player[0].health,_player[0].x + 8,_player[0].height / 2 + _player[0].y,5);
      _damage_player(1,_player[1].health,_player[1].x + 8,_player[1].height / 2 + _player[1].y,5);
    }
  }
  if (_BLOODBATH != 0) {
    _BLOODBATH = _BLOODBATH + 1;
    if (0x27f < _BLOODBATH) {
      _BLOODBATH = 1;
    }
    if ((_BLOODBATH < 0x131) || (0x14f < _BLOODBATH)) {
      _spawn_particles(_BLOODBATH,1,0x20,0,0);
    }
    else if (_BLOODBATH == 0x140) {
      type = 2;
      color = 0;
      iVar1 = _rand();
      _spawn_particles(0x140,1,iVar1 % 2 + 1,color,type);
    }
  }
  return;
}

