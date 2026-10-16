/* Original CELL.EXE address: 00008ad0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _adjust_player(Player *p,Player *other,int index)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char local_14;
  
  if (p->state == 0) {
    p->frame = 0x138;
  }
  if (p->roll_wait != 0) {
    p->roll_wait = p->roll_wait + -1;
  }
  if (p->health < '\x01') {
    _endmatch = _endmatch + -1;
  }
  switch(p->state) {
  case 5:
    if (((p->vx == 0) && (p->vy == 0)) && (iVar2 = _hit(p->x + 8,p->y + 0x19), iVar2 == 1)) {
      p->state = 0;
      p->frame = 0x138;
      p->height = 0x19;
      iVar2 = p->y;
      if (iVar2 < 0) {
        iVar2 = iVar2 + 0x1f;
      }
      p->y = ((iVar2 >> 5) * 0x20 + 0x20) - p->height;
    }
    else if (p->vy < 0) {
      p->frame = p->frame + 1;
      if (p->frame < 0x7f) {
        p->frame = 0x7f;
      }
      else if (0x8d < p->frame) {
        p->frame = 0x8d;
      }
    }
    else if (0 < p->vy) {
      p->frame = p->frame + 1;
      if (p->frame < 0x8d) {
        p->frame = 0x8d;
      }
      else if (0x9c < p->frame) {
        p->frame = 0x9c;
      }
    }
    break;
  case 6:
    if ((p->weapon == 1) && (p->frame == 0xb2)) {
      if (('\0' < other->health) &&
         ((((p->x < other->x && (p->facing != '\0')) || ((other->x < p->x && (p->facing == '\0'))))
          && (iVar2 = _distance(p->x + 8,p->height / 2 + p->y,other->x + 8,
                                other->height / 2 + other->y), iVar2 < 0x22)))) {
        iVar2 = p->x;
        iVar1 = _rand();
        _play_sound(iVar1 % 3 + 0x165,iVar2);
        iVar2 = _hit(other->x + 8,other->y + -3);
        if (iVar2 != 1) {
          other->y = other->y + -3;
        }
        other->vy = -3;
        if (other->x < p->x) {
          other->vx = -6;
        }
        else {
          other->vx = 6;
        }
        other->facing = p->facing;
        other->state = 5;
        other->frame = 0x7f;
        if (index == 0) {
          iVar4 = 2;
          iVar3 = other->height / 2 + other->y;
          iVar1 = other->x + 8;
          iVar2 = _rand();
          local_14 = (char)iVar2 + (char)(iVar2 / 5) * -5;
          _damage_player(1,local_14 + '\n',iVar1,iVar3,iVar4);
        }
        else {
          iVar4 = 2;
          iVar2 = other->height / 2 + other->y;
          iVar3 = other->x + 8;
          iVar1 = _rand();
          local_14 = (char)iVar1 + (char)(iVar1 / 5) * -5;
          _damage_player(0,local_14 + '\n',iVar3,iVar2,iVar4);
        }
      }
    }
    else if (((p->weapon == 3) && ((0x11f < p->frame && (p->frame < 0x134)))) &&
            ((((p->facing != '\0' && (p->x < other->x)) ||
              ((p->facing == '\0' && (other->x < p->x)))) &&
             (iVar2 = _distance(p->x + 8,p->height / 2 + p->y,other->x + 8,
                                other->height / 2 + other->y), iVar2 < 0x3a)))) {
      other->stun = 0x46;
    }
    break;
  case 7:
    if (p->frame < 0x49) {
      p->frame = (int)ROUND(_LC51 + (_LC50 - _LC49 * (double)_endmatch));
      if (p->frame < 0x2c) {
        p->frame = 0x2c;
      }
      else if (0x49 < p->frame) {
        p->frame = 0x49;
      }
    }
    break;
  case 8:
    if (p->frame < 0x2b) {
      p->frame = (int)ROUND(_LC52 + (_LC50 - _LC49 * (double)_endmatch));
      if (p->frame < 0xe) {
        p->frame = 0xe;
      }
      else if (0x2b < p->frame) {
        p->frame = 0x2b;
      }
    }
    break;
  case 9:
    if (p->frame < 0x11e) {
      p->frame = (int)ROUND(_LC53 + (_LC50 - _LC49 * (double)_endmatch));
      if (p->frame < 0x10b) {
        p->frame = 0x10b;
      }
      else if (0x11e < p->frame) {
        p->frame = 0x11e;
      }
    }
    break;
  case 10:
    if (p->frame < 0x49) {
      p->frame = (int)ROUND(_LC51 + (_LC50 - _LC49 * (double)_endmatch));
      if (p->frame < 0x2c) {
        p->frame = 0x2c;
      }
      else if (0x49 < p->frame) {
        p->frame = 0x49;
      }
    }
  }
  if (p->attack_wait < 1) {
    if (p->state == 6) {
      p->state = 0;
      p->frame = 0x138;
    }
  }
  else {
    iVar2 = p->weapon;
    if (iVar2 == 1) {
      p->frame = p->frame + 1;
      if (0xbf < p->frame) {
        p->frame = 0xbf;
      }
    }
    else if (iVar2 == 0) {
      p->frame = p->frame + 1;
      if (0xab < p->frame) {
        p->frame = 0x9d;
      }
    }
    else if (iVar2 == 2) {
      p->frame = p->frame + 1;
      if (0x10a < p->frame) {
        p->frame = 0x10a;
      }
    }
    else if (iVar2 == 3) {
      p->frame = p->frame + 1;
      if ((p->frame < 0x7d) && (0x68 < p->frame)) {
        if (0x7c < p->frame) {
          p->frame = 0x7c;
        }
        if ((((p->frame == 0x77) && ('\0' < other->health)) &&
            (iVar2 = _distance(p->x + 8,p->height / 2 + p->y,other->x + 8,
                               other->height / 2 + other->y), iVar2 < 0x1e)) &&
           (((p->facing != '\0' && (p->x < other->x)) || ((p->facing == '\0' && (other->x < p->x))))
           )) {
          iVar2 = p->x;
          iVar1 = _rand();
          _play_sound(iVar1 % 2 + 0x15c,iVar2);
          if (other->x < p->x) {
            other->vx = -4;
          }
          else {
            other->vx = 4;
          }
          other->facing = p->facing;
          other->state = 3;
          other->frame = 0xc0;
          other->height = 0xd;
          iVar2 = other->y;
          if (iVar2 < 0) {
            iVar2 = iVar2 + 0x1f;
          }
          other->y = ((iVar2 >> 5) * 0x20 + 0x20) - other->height;
          if (index == 0) {
            iVar4 = 2;
            iVar3 = other->y + 0xd;
            iVar2 = other->x;
            iVar1 = _rand();
            local_14 = (char)iVar1 + (char)(iVar1 / 7) * -7;
            _damage_player(1,local_14 + '\a',iVar2,iVar3,iVar4);
          }
          else {
            iVar4 = 2;
            iVar3 = other->y + 0xd;
            iVar2 = other->x;
            iVar1 = _rand();
            local_14 = (char)iVar1 + (char)(iVar1 / 7) * -7;
            _damage_player(0,local_14 + '\a',iVar2,iVar3,iVar4);
          }
        }
      }
      else if (0x137 < p->frame) {
        p->frame = 0x137;
      }
    }
    p->attack_wait = p->attack_wait + -1;
  }
  iVar2 = _hit(p->x + 8,p->y + p->height);
  if ((iVar2 == 0) || ((iVar2 = _hit(p->x + 8,p->y + p->height), iVar2 == 2 && (p->state != 4)))) {
    if ((p->state == 4) || ((p->state == 2 || (p->state == 0)))) {
      p->height = 0x19;
      p->state = 5;
      p->frame = 0x7f;
    }
    if (p->field54 == 1) {
      if (p->vx < 4) {
        if (p->vx < -3) {
          p->vx = p->vx + 1;
        }
      }
      else {
        p->vx = p->vx + -1;
      }
      if (p->vy < 6) {
        p->vy = p->vy + 1;
      }
      p->field54 = 0;
    }
    else {
      p->field54 = 1;
    }
    if (p->fallen == 0) {
      iVar2 = _hit(p->x + 8,p->y + p->vy + p->height / 2);
      if (iVar2 == 1) {
        p->vy = 0;
      }
      else {
        p->y = p->vy + p->y;
      }
      iVar2 = _hit(p->x + p->vx + 0x11,p->height / 2 + p->y);
      if (((iVar2 == 1) && (0 < p->vx)) ||
         ((iVar2 = _hit(p->x + p->vx + -1,p->height / 2 + p->y), iVar2 == 1 && (p->vx < 0)))) {
        p->vx = 0;
      }
      else {
        p->x = p->vx + p->x;
      }
    }
    iVar2 = _hit(p->x + 8,p->y + p->height);
    if (iVar2 == 1) {
      iVar2 = p->y;
      if (iVar2 < 0) {
        iVar2 = iVar2 + 0x1f;
      }
      p->y = ((iVar2 >> 5) * 0x20 + 0x20) - p->height;
      p->vx = 0;
      p->vy = 0;
      if (((((p->state == 5) || (p->state == 4)) || (p->state == 3)) ||
          ((p->state == 1 || (p->state == 6)))) || (p->state == 2)) {
        p->state = 0;
        p->frame = 0x138;
      }
    }
  }
  if ((p->vx != 0) && (p->field10 == p->x)) {
    p->vx = 0;
  }
  if ((p->vy != 0) && (p->field14 == p->y)) {
    p->vy = 0;
  }
  p->field10 = p->x;
  p->field14 = p->y;
  return;
}

