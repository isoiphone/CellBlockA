/* Original 1.3b CELL.EXE address: 0000a2d0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _translate_player(int index)

{
  int iVar1;
  int iVar2;
  int y;
  Player *p;
  int reason;
  char local_28;
  uint local_18;
  Player *local_14;
  
  p = _player + index;
  if (index == 0) {
    local_14 = _player + 1;
  }
  else {
    local_14 = _player;
  }
  if (0 < local_14->stun) {
    local_14->stun = local_14->stun + -1;
  }
  if ((_player[index].health != '\0') && (_player[index].stun == 0)) {
    _control_player(p,local_14);
  }
  _adjust_player(p,local_14,index);
  if (((_player[index].height != 0x19) && (_player[index].state != 1)) &&
     (_player[index].state != 3)) {
    _player[index].height = 0x19;
    iVar2 = _player[index].y;
    if (iVar2 < 0) {
      iVar2 = iVar2 + 0x1f;
    }
    _player[index].y = ((iVar2 >> 5) * 0x20 + 0x20) - _player[index].height;
  }
  if (((_player[index].health != '\0') && (iVar2 = _hit_player(index), iVar2 != 0)) &&
     (_player[index].state != 3)) {
    if (index == 0) {
      local_14 = (Player *)0x816cc;
    }
    else {
      local_14 = _player;
    }
    local_18 = (uint)(index == 0);
    if ((_player[index].y + 0x19 <= local_14->y + 6) && (_player[index].vy == 6)) {
      if ((local_14->state != 1) && ((local_14->state != 3 && (local_14->state != 4)))) {
        _player[index].vy = 0;
        _crunchsound(_player[index].x + 8);
        _damage_player(local_18,local_14->health,local_14->x,local_14->y + 8,1);
        return;
      }
      _player[index].y = _player[index].y + -3;
      _player[index].vy = 0;
      _crunchsound(_player[index].x + 8);
      reason = 1;
      y = local_14->y + 8;
      iVar2 = local_14->x;
      iVar1 = _rand();
      local_28 = (char)iVar1 + (char)(iVar1 / 10) * -10;
      _damage_player(local_18,local_28 + '\x14',iVar2,y,reason);
      return;
    }
    _player[index].vy = 0;
    if (local_14->state == 1) {
      iVar2 = _hit(_player[index].x + 8,_player[index].y + -0xf);
      if (iVar2 != 1) {
        _player[index].y = _player[index].y + -0xf;
      }
      _player[index].vy = -4;
      if (_player[index].facing == '\0') {
        _player[index].x = _player[index].x + -3;
        _player[index].vx = -8;
      }
      else {
        _player[index].x = _player[index].x + 3;
        _player[index].vx = 8;
      }
    }
    else if (local_14->state == 3) {
      iVar2 = _hit(_player[index].x + 8,_player[index].y + -0xf);
      if (iVar2 != 1) {
        _player[index].y = _player[index].y + -0xf;
      }
      _player[index].vy = -4;
      if (local_14->facing == '\0') {
        _player[index].facing = '\x01';
        _player[index].x = _player[index].x + 3;
        _player[index].vx = 8;
      }
      else {
        _player[index].facing = '\0';
        _player[index].x = _player[index].x + -3;
        _player[index].vx = -8;
      }
    }
    else {
      iVar2 = _hit(_player[index].x + 8,_player[index].y + -3);
      if (iVar2 != 1) {
        _player[index].y = _player[index].y + -3;
      }
      iVar2 = _hit(local_14->x + 8,local_14->y + -3);
      if (iVar2 != 1) {
        local_14->y = local_14->y + -3;
      }
      local_14->vy = -3;
      if (local_14->x < _player[index].x) {
        _player[index].vx = 3;
        local_14->vx = -8;
      }
      else {
        _player[index].vx = -3;
        local_14->vx = 8;
      }
      local_14->facing = _player[index].facing;
      local_14->state = 5;
      local_14->frame = 0x82;
    }
    _player[index].state = 5;
    _player[index].frame = 0x82;
  }
  if (_player[index].x < 0) {
    _player[index].x = 0;
    _player[index].vx = (int)ROUND(_DAT_0000a2c8 * (double)_player[index].vx);
  }
  else if (0x27f < _player[index].x + 0x10) {
    _player[index].x = 0x26e;
    _player[index].vx = (int)ROUND(_DAT_0000a2c8 * (double)_player[index].vx);
  }
  if (_player[index].y < 0) {
    _player[index].y = 0;
  }
  else if (0x1af < _player[index].y + _player[index].height) {
    _crunchsound(_player[index].x + 8);
    _player[index].y = 0x196;
    _player[index].vy = 0;
    _player[index].vx = 0;
    _damage_player(index,_player[index].health,_player[index].x + 8,_player[index].y + 0x18,0);
    p->fallen = 1;
  }
  return;
}

