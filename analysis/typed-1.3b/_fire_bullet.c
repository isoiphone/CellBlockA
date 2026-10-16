/* Original 1.3b CELL.EXE address: 00006c9c */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _fire_bullet(int param_1,float param_2,float param_3,float param_4)

{
  float fVar1;
  int iVar2;
  int iVar3;
  int reason;
  int iVar4;
  int local_2c;
  int local_c;
  uint local_8;
  
  local_8 = (uint)(param_1 == 0);
  if (_player[param_1].facing == '\0') {
    param_2 = param_2 - 1.0;
    local_c = -5;
  }
  else {
    param_2 = param_2 + _DAT_00006c90;
    local_c = 5;
  }
  for (; 0.0 < param_2; param_2 = param_2 + (float)local_c) {
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      if (0.0 <= param_2) break;
    }
    else {
      fVar1 = (float)_gfx_driver->w;
      if ((byte)(param_2 < fVar1 | (byte)((ushort)((ushort)(NAN(param_2) || NAN(fVar1)) << 10) >> 8)
                | (byte)((ushort)((ushort)(param_2 == fVar1) << 0xe) >> 8)) != 1) break;
    }
    if (param_3 <= 0.0) break;
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      if (0.0 <= param_3) break;
    }
    else {
      fVar1 = (float)_gfx_driver->h;
      if ((byte)(param_3 < fVar1 | (byte)((ushort)((ushort)(NAN(param_3) || NAN(fVar1)) << 10) >> 8)
                | (byte)((ushort)((ushort)(param_3 == fVar1) << 0xe) >> 8)) != 1) break;
    }
    fVar1 = (float)_player[local_8].x;
    if ((((0x3f < (byte)((param_2 < fVar1 |
                          (byte)((ushort)((ushort)(NAN(param_2) || NAN(fVar1)) << 10) >> 8) |
                         (byte)((ushort)((ushort)(param_2 == fVar1) << 0xe) >> 8)) - 1)) &&
         (param_2 < (float)(_player[local_8].x + 0x10))) &&
        (fVar1 = (float)_player[local_8].y,
        0x3f < (byte)((param_3 < fVar1 |
                       (byte)((ushort)((ushort)(NAN(param_3) || NAN(fVar1)) << 10) >> 8) |
                      (byte)((ushort)((ushort)(param_3 == fVar1) << 0xe) >> 8)) - 1))) &&
       (param_3 < (float)(_player[local_8].y + _player[local_8].height))) {
      reason = 3;
      iVar2 = (int)ROUND(param_3);
      iVar3 = (int)ROUND(param_2);
      iVar4 = _rand();
      local_2c._0_1_ = (char)iVar4 + (char)(iVar4 / 0x19) * -0x19;
      _damage_player(local_8,(char)local_2c + '2',iVar3,iVar2,reason);
      if (_player[local_8].health < '\x01') {
        return;
      }
      _player[local_8].y = _player[local_8].y + -4;
      _player[local_8].vy = _player[local_8].vy + (int)ROUND(param_4);
      _player[local_8].vx = local_c + _player[local_8].vx;
      return;
    }
    param_3 = param_3 + param_4;
  }
  if (1.0 < param_2) {
    local_2c = (int)ROUND(param_2);
    if (local_2c < 0) {
      local_2c = local_2c + 0x1f;
    }
    param_2 = (float)((local_2c >> 5) << 5);
    if (_player[param_1].facing == '\0') {
      param_2 = param_2 + _DAT_00006c98;
    }
    else {
      param_2 = param_2 - _DAT_00006c94;
    }
  }
  iVar4 = 0;
  iVar3 = 2;
  iVar2 = _rand();
  _spawn_particles((int)ROUND(param_2),(int)ROUND(param_3),iVar2 % 2 + 3,iVar3,iVar4);
  return;
}

