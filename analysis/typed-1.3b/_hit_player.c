/* Original 1.3b CELL.EXE address: 00008028 */

undefined4 _hit_player(int param_1)

{
  undefined4 uVar1;
  uint local_8;
  
  local_8 = (uint)(param_1 == 0);
  if (_player[local_8].health == '\0') {
    uVar1 = 0;
  }
  else if ((((_player[local_8].x + 0x10 < _player[param_1].x) ||
            (_player[param_1].x + 0x10 < _player[local_8].x)) ||
           (_player[local_8].y + _player[local_8].height < _player[param_1].y)) ||
          (_player[param_1].y + _player[param_1].height < _player[local_8].y)) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}

