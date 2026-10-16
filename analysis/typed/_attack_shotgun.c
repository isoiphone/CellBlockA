/* Original CELL.EXE address: 00006d94 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _attack_shotgun(int param_1)

{
  int iVar1;
  int local_8;
  
  _player[param_1].attack_wait = 0x23;
  _player[param_1].ammo = _player[param_1].ammo + -1;
  if (_player[param_1].facing == '\0') {
    _play_sound(0x16a,_player[param_1].x + -2);
  }
  else {
    _play_sound(0x16a,_player[param_1].x + 0x12);
  }
  for (local_8 = 0; local_8 < 3; local_8 = local_8 + 1) {
    iVar1 = _rand();
    _fire_bullet(param_1,(float)_player[param_1].x,(float)(_player[param_1].y + 0xd),
                 -(_LC46 / (float)(iVar1 % 0x3ac + 0x3c)));
  }
  return;
}

