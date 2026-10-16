/* Original CELL.EXE address: 00006fb8 */

void _spawn_player(int param_1)

{
  int iVar1;
  int iVar2;
  int local_c;
  
  local_c = _rand();
  iVar1 = local_c;
  if (local_c < 0) {
    iVar1 = local_c + 3;
  }
  local_c = local_c + (iVar1 >> 2) * -4;
  while ((((char)((char)_start[local_c].packed * '\x02') >> 1 == -1 ||
          ((short)(_start[local_c].packed << 2) >> 9 == -1)) || (_lastspot == local_c))) {
    iVar1 = local_c + 1;
    iVar2 = iVar1;
    if (iVar1 < 0) {
      iVar2 = local_c + 4;
    }
    local_c = iVar1 + (iVar2 >> 2) * -4;
  }
  _lastspot = local_c;
  _player[param_1].x = (int)((char)((char)_start[local_c].packed * '\x02') >> 1) << 5;
  _player[param_1].y = (int)((short)(_start[local_c].packed << 2) >> 9) << 5;
  _player[param_1].height = 0x19;
  _player[param_1].vx = 0;
  _player[param_1].vy = 0;
  _player[param_1].facing = *(byte *)((int)&_start[local_c].packed + 1) >> 6 & 1;
  _player[param_1].frame = 0x138;
  _player[param_1].state = 0;
  _player[param_1].health = 'c';
  _player[param_1].weapon = 0;
  _player[param_1].attack_wait = 0;
  _player[param_1].ammo = 0;
  _player[param_1].roll_wait = 0;
  _player[param_1].stun = 0;
  _player[param_1].fallen = 0;
  return;
}

