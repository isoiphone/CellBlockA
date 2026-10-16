/* Original CELL.EXE address: 000066b0 */

void _attack_fireext(int param_1)

{
  int iVar1;
  uint local_8;
  
  local_8 = (uint)(param_1 == 0);
  if ((_player[param_1].ammo < 1) ||
     (iVar1 = _distance(_player[param_1].x + 8,_player[param_1].y + _player[param_1].height / 2,
                        _player[local_8].x + 8,_player[local_8].y + _player[local_8].height / 2),
     iVar1 < 0x1d)) {
    _player[param_1].frame = 0x69;
    _player[param_1].attack_wait = 0x12;
    _play_sound(0x168,_player[param_1].x);
  }
  else {
    _player[param_1].frame = 0x11f;
    _player[param_1].attack_wait = 0x1e;
    _player[param_1].ammo = _player[param_1].ammo + -1;
    _play_sound(0x15f,_player[param_1].x);
  }
  return;
}

