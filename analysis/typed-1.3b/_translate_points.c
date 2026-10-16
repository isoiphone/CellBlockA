/* Original 1.3b CELL.EXE address: 000025a4 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _translate_points(void)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_54;
  int local_8;
  
  local_8 = 0;
  do {
    if (0xff < local_8) {
      return;
    }
    if (0 < _point[local_8].life) {
      _point[local_8].life = _point[local_8].life + -1;
      _point[local_8].x = _point[local_8].vx + _point[local_8].x;
      iVar2 = _fixtoi(_point[local_8].y);
      iVar3 = _fixtoi(_point[local_8].x);
      iVar2 = _hit(iVar3,iVar2);
      if (iVar2 == 1) {
        _point[local_8].vx = (int)ROUND(_DAT_00002594 * (double)_point[local_8].vx);
        _point[local_8].vy = (int)ROUND(_DAT_00002594 * (double)_point[local_8].vy);
        iVar2 = _point[local_8].vx;
        if (iVar2 < 0) {
          iVar2 = -iVar2;
        }
        iVar3 = _ftofix(0.0);
        if (iVar2 < iVar3) {
          iVar2 = _point[local_8].vy;
          if (iVar2 < 0) {
            iVar2 = -iVar2;
          }
          iVar3 = _ftofix(0.0);
          if ((iVar2 < iVar3) && (_point[local_8].life < 10)) {
            _point[local_8].life = 0;
          }
        }
      }
      else if (_point[local_8].type == 1) {
        iVar2 = _ftofix(0.5);
        _point[local_8].y = iVar2 + _point[local_8].y;
        iVar2 = _ftofix(1.1);
        _point[local_8].y = _point[local_8].vy / iVar2 + _point[local_8].y;
        iVar2 = _ftofix(0.1);
        _point[local_8].vy = iVar2 + _point[local_8].vy;
      }
      else {
        iVar2 = _ftofix(0.15);
        iVar2 = iVar2 + _point[local_8].vy;
        _point[local_8].vy = iVar2;
        _point[local_8].y = iVar2 + _point[local_8].y;
      }
      if (_point[local_8].x < 0) {
        _point[local_8].x = 0;
        _point[local_8].vx = (int)ROUND(_DAT_00002594 * (double)_point[local_8].vx);
      }
      else {
        iVar2 = _fixtoi(_point[local_8].x);
        if (0x27f < iVar2) {
          iVar2 = _itofix(0x27f);
          _point[local_8].x = iVar2;
          _point[local_8].vx = (int)ROUND(_DAT_00002594 * (double)_point[local_8].vx);
        }
      }
      if (_point[local_8].y < 0) {
        _point[local_8].y = 0;
        _point[local_8].vy = (int)ROUND(_DAT_00002594 * (double)_point[local_8].vy);
      }
      else {
        iVar2 = _fixtoi(_point[local_8].y);
        if (0x195 < iVar2) {
          _point[local_8].life = -1;
          iVar2 = _itofix(0x195);
          _point[local_8].y = iVar2;
          _point[local_8].vy = 0;
        }
      }
      uVar1 = _point[local_8].type;
      if (uVar1 == 2) {
        iVar2 = _fixtoi(_point[local_8].y);
        iVar3 = _fixtoi(_point[local_8].x);
        iVar2 = _hit(iVar3,iVar2);
        if (iVar2 == 1) {
          local_54 = _fixtoi(_point[local_8].y);
          if (local_54 < 0) {
            local_54 = local_54 + 0x1f;
          }
          iVar2 = _itofix((local_54 >> 5) * 0x20 + -1);
          _point[local_8].y = iVar2;
          _point[local_8].vy = 0;
          _point[local_8].vx = 0;
        }
        iVar5 = 6;
        iVar4 = 0x20;
        iVar2 = _fixtoi(_point[local_8].y);
        iVar3 = _fixtoi(_point[local_8].x);
        iVar2 = _collide(_player[0].x,_player[0].y,0x10,_player[0].height,iVar3,iVar2,iVar4,iVar5);
        if (iVar2 != 0) {
          iVar2 = _fixtoi(_point[local_8].x);
          _play_sound(0x1ad,iVar2);
          _player[0].weapon = 2;
          _player[0].ammo = 6;
          _point[local_8].life = -1;
          return;
        }
        iVar5 = 6;
        iVar4 = 0x20;
        iVar2 = _fixtoi(_point[local_8].y);
        iVar3 = _fixtoi(_point[local_8].x);
        iVar2 = _collide(_player[1].x,_player[1].y,0x10,_player[1].height,iVar3,iVar2,iVar4,iVar5);
        if (iVar2 != 0) {
          iVar2 = _fixtoi(_point[local_8].x);
          _play_sound(0x1ad,iVar2);
          _player[1].weapon = 2;
          _player[1].ammo = 6;
          _point[local_8].life = -1;
          return;
        }
      }
      else if (uVar1 < 3) {
        if (uVar1 == 1) {
          iVar2 = _fixtoi(_point[local_8].y);
          iVar3 = _fixtoi(_point[local_8].x);
          iVar2 = _hit(iVar3,iVar2);
          if (iVar2 == 1) {
            local_54 = _fixtoi(_point[local_8].y);
            if (local_54 < 0) {
              local_54 = local_54 + 0x1f;
            }
            iVar2 = _itofix((local_54 >> 5) * 0x20 + -1);
            _point[local_8].y = iVar2;
            _point[local_8].vx = (int)ROUND(_DAT_00002594 * (double)_point[local_8].vx);
            _point[local_8].vy = (int)ROUND(_DAT_0000259c * (double)_point[local_8].vy);
          }
        }
      }
      else if (uVar1 == 3) {
        iVar2 = _fixtoi(_point[local_8].y);
        iVar3 = _fixtoi(_point[local_8].x);
        iVar2 = _hit(iVar3,iVar2);
        if (iVar2 == 1) {
          local_54 = _fixtoi(_point[local_8].y);
          if (local_54 < 0) {
            local_54 = local_54 + 0x1f;
          }
          iVar2 = _itofix((local_54 >> 5) * 0x20 + -1);
          _point[local_8].y = iVar2;
          _point[local_8].vy = 0;
          _point[local_8].vx = 0;
        }
        iVar5 = 3;
        iVar4 = 0x20;
        iVar2 = _fixtoi(_point[local_8].y);
        iVar3 = _fixtoi(_point[local_8].x);
        iVar2 = _collide(_player[0].x,_player[0].y,0x10,_player[0].height,iVar3,iVar2,iVar4,iVar5);
        if (iVar2 != 0) {
          iVar2 = _fixtoi(_point[local_8].x);
          _play_sound(0x1b7,iVar2);
          _player[0].weapon = 1;
          _point[local_8].life = -1;
          return;
        }
        iVar5 = 3;
        iVar4 = 0x20;
        iVar2 = _fixtoi(_point[local_8].y);
        iVar3 = _fixtoi(_point[local_8].x);
        iVar2 = _collide(_player[1].x,_player[1].y,0x10,_player[1].height,iVar3,iVar2,iVar4,iVar5);
        if (iVar2 != 0) {
          iVar2 = _fixtoi(_point[local_8].x);
          _play_sound(0x1b7,iVar2);
          _player[1].weapon = 1;
          _point[local_8].life = -1;
          return;
        }
      }
      else if (uVar1 == 4) {
        iVar2 = _fixtoi(_point[local_8].y);
        iVar3 = _fixtoi(_point[local_8].x);
        iVar2 = _hit(iVar3,iVar2);
        if (iVar2 == 1) {
          local_54 = _fixtoi(_point[local_8].y);
          if (local_54 < 0) {
            local_54 = local_54 + 0x1f;
          }
          iVar2 = _itofix((local_54 >> 5) * 0x20 + -1);
          _point[local_8].y = iVar2;
          _point[local_8].vy = 0;
          _point[local_8].vx = 0;
        }
        iVar5 = 0xc;
        iVar4 = 0x1b;
        iVar2 = _fixtoi(_point[local_8].y);
        iVar3 = _fixtoi(_point[local_8].x);
        iVar2 = _collide(_player[0].x,_player[0].y,0x10,_player[0].height,iVar3,iVar2,iVar4,iVar5);
        if (iVar2 != 0) {
          iVar2 = _fixtoi(_point[local_8].x);
          _play_sound(0x1b1,iVar2);
          _player[0].weapon = 3;
          _player[0].ammo = 5;
          _point[local_8].life = -1;
          return;
        }
        iVar5 = 0xc;
        iVar4 = 0x1b;
        iVar2 = _fixtoi(_point[local_8].y);
        iVar3 = _fixtoi(_point[local_8].x);
        iVar2 = _collide(_player[1].x,_player[1].y,0x10,_player[1].height,iVar3,iVar2,iVar4,iVar5);
        if (iVar2 != 0) {
          iVar2 = _fixtoi(_point[local_8].x);
          _play_sound(0x1b1,iVar2);
          _player[1].weapon = 3;
          _player[1].ammo = 5;
          _point[local_8].life = -1;
          return;
        }
      }
    }
    local_8 = local_8 + 1;
  } while( true );
}

