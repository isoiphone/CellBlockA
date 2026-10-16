/* Original CELL.EXE address: 00006134 */

void _attack_fists(int param_1)

{
  int iVar1;
  int x;
  int y;
  int reason;
  char local_10;
  uint local_8;
  
  local_8 = (uint)(param_1 == 0);
  _player[param_1].attack_wait = 0x12;
  _play_sound(0x162,_player[param_1].x);
  if ((('\0' < _player[local_8].health) &&
      (((_player[param_1].x < _player[local_8].x && (_player[param_1].facing != '\0')) ||
       ((_player[local_8].x < _player[param_1].x && (_player[param_1].facing == '\0')))))) &&
     (iVar1 = _distance(_player[param_1].x + 8,_player[param_1].height / 2 + _player[param_1].y,
                        _player[local_8].x + 8,_player[local_8].height / 2 + _player[local_8].y),
     iVar1 < 0x1c)) {
    _play_sound(0x163,_player[param_1].x);
    iVar1 = _hit(_player[local_8].x + 8,_player[local_8].y + -3);
    if (iVar1 != 1) {
      _player[local_8].y = _player[local_8].y + -3;
    }
    _player[local_8].vy = -3;
    if (_player[local_8].x < _player[param_1].x) {
      _player[local_8].vx = -4;
    }
    else {
      _player[local_8].vx = 4;
    }
    _player[local_8].facing = _player[param_1].facing;
    _player[local_8].state = 5;
    _player[local_8].frame = 0x7f;
    reason = 2;
    y = _player[local_8].height / 2 + _player[local_8].y;
    x = _player[local_8].x + 8;
    iVar1 = _rand();
    local_10 = (char)iVar1 + (char)(iVar1 / 3) * -3;
    _damage_player(local_8,local_10 + '\x05',x,y,reason);
  }
  return;
}

