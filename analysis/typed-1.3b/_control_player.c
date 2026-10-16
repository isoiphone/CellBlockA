/* Original 1.3b CELL.EXE address: 000086b0 */

void _control_player(Player *p,Player *other)

{
  int iVar1;
  
  if ((p->down == 0) && (p->state == 1)) {
    p->state = 0;
    p->frame = 0x13b;
    p->height = 0x19;
    iVar1 = p->y;
    if (iVar1 < 0) {
      iVar1 = iVar1 + 0x1f;
    }
    p->y = ((iVar1 >> 5) * 0x20 + 0x20) - p->height;
  }
  else if (p->state == 3) {
    if (p->facing == '\0') {
      iVar1 = _hit(p->x + -3,p->y + 2);
      if ((iVar1 == 1) || (iVar1 = _hit(p->x + -4,p->y + 0xc), iVar1 == 1)) {
        p->frame = 0xd1;
        iVar1 = p->x;
        if (iVar1 < 0) {
          iVar1 = iVar1 + 0x1f;
        }
        p->x = (iVar1 >> 5) << 5;
      }
      else {
        p->vx = -3;
        p->x = p->x + -3;
      }
    }
    else {
      iVar1 = _hit(p->x + 0x13,p->y + 2);
      if ((iVar1 == 1) || (iVar1 = _hit(p->x + 0x14,p->y + 0xc), iVar1 == 1)) {
        p->frame = 0xd1;
        iVar1 = p->x;
        if (iVar1 < 0) {
          iVar1 = iVar1 + 0x1f;
        }
        p->x = (iVar1 >> 5) * 0x20 + 0x10;
      }
      else {
        p->vx = 3;
        p->x = p->x + 3;
      }
    }
    p->frame = p->frame + 1;
    if (0xd1 < p->frame) {
      p->roll_wait = 0xc;
      p->state = 0;
      p->frame = 0x13b;
      p->height = 0x19;
      iVar1 = p->y;
      if (iVar1 < 0) {
        iVar1 = iVar1 + 0x1f;
      }
      p->y = ((iVar1 >> 5) * 0x20 + 0x20) - p->height;
    }
    p->up = 0;
    p->down = 0;
    p->left = 0;
    p->right = 0;
  }
  if ((p->down == 0) || (p->state == 6)) {
    if ((p->up != 0) && (p->state != 6)) {
      if (((p->state == 4) || (p->state == 0)) && (iVar1 = _hit(p->x + 8,p->y + -1), iVar1 == 2)) {
        if (p->state == 4) {
          p->y = p->y + -1;
          p->frame = p->frame + -1;
          if (p->frame < 0x4a) {
            p->frame = 0x67;
          }
        }
        else {
          p->y = p->y + -3;
          p->vx = 0;
          p->vy = 0;
          p->state = 4;
          p->frame = 0x4a;
        }
      }
      else {
        iVar1 = _hit(p->x + 8,p->y + p->height);
        if (((iVar1 != 0) && (iVar1 = _hit(p->x + 8,p->y + -1), iVar1 != 1)) &&
           ((p->state == 0 || (p->state == 2)))) {
          if (p->left == 0) {
            if (p->right == 0) {
              p->vx = 0;
            }
            else {
              p->facing = '\x01';
              p->vx = 6;
            }
          }
          else {
            p->facing = '\0';
            p->vx = -6;
          }
          p->state = 5;
          p->frame = 0x82;
          p->vy = -6;
          p->y = p->y + -3;
        }
      }
    }
  }
  else {
    iVar1 = _hit(p->x + 8,p->y + p->height + 1);
    if ((iVar1 == 2) && ((p->state == 0 || (p->state == 4)))) {
      if (p->state == 4) {
        p->y = p->y + 1;
        p->frame = p->frame + 1;
        if (0x67 < p->frame) {
          p->frame = 0x4a;
        }
      }
      else {
        p->vx = 0;
        p->vy = 0;
        p->state = 4;
        p->frame = 0x4a;
      }
    }
    else {
      iVar1 = _hit(p->x + 8,p->y + p->height);
      if ((iVar1 == 1) && ((p->state == 0 || (p->state == 2)))) {
        if (p->left == 0) {
          if (p->right == 0) {
            p->state = 1;
            p->frame = 0x6b;
            p->height = 0xd;
            iVar1 = p->y;
            if (iVar1 < 0) {
              iVar1 = iVar1 + 0x1f;
            }
            p->y = ((iVar1 >> 5) * 0x20 + 0x20) - p->height;
          }
          else if (p->roll_wait == 0) {
            p->facing = '\x01';
            p->state = 3;
            p->frame = 0xc3;
            p->height = 0xd;
            iVar1 = p->y;
            if (iVar1 < 0) {
              iVar1 = iVar1 + 0x1f;
            }
            p->y = ((iVar1 >> 5) * 0x20 + 0x20) - p->height;
          }
        }
        else if (p->roll_wait == 0) {
          p->facing = '\0';
          p->state = 3;
          p->frame = 0xc3;
          p->height = 0xd;
          iVar1 = p->y;
          if (iVar1 < 0) {
            iVar1 = iVar1 + 0x1f;
          }
          p->y = ((iVar1 >> 5) * 0x20 + 0x20) - p->height;
        }
        p->left = 0;
        p->right = 0;
      }
    }
  }
  iVar1 = _hit(p->x + 8,p->y + p->height);
  if ((iVar1 == 1) || ((iVar1 = _hit(p->x + 8,p->y + p->height), iVar1 == 2 && (p->state != 5)))) {
    if ((p->left == 0) || (p->state == 6)) {
      if ((p->right != 0) && (p->state != 6)) {
        p->facing = '\x01';
        iVar1 = _hit(p->x + 0x12,p->y + 0xc);
        if ((iVar1 != 1) && (iVar1 = _hit(p->x + 0x12,p->y + 0x18), iVar1 != 1)) {
          iVar1 = _hit(p->x + 8,p->y + p->height);
          if (iVar1 == 1) {
            iVar1 = p->y;
            if (iVar1 < 0) {
              iVar1 = iVar1 + 0x1f;
            }
            p->y = ((iVar1 >> 5) * 0x20 + 0x20) - p->height;
          }
          if ((((p->state != 2) && (p->state != 4)) && (p->state != 1)) && (p->state != 3)) {
            p->state = 2;
            p->frame = 0xd2;
          }
          if (p->x + 0x11 < 0x27f) {
            p->frame = p->frame + 1;
            if (p->state == 4) {
              if (0x67 < p->frame) {
                p->frame = 0x4a;
              }
            }
            else if (0xef < p->frame) {
              p->frame = 0xd2;
            }
            p->x = p->x + 2;
            p->vx = 2;
          }
        }
      }
    }
    else {
      p->facing = '\0';
      iVar1 = _hit(p->x + -2,p->y + 0xc);
      if ((iVar1 != 1) && (iVar1 = _hit(p->x + -2,p->y + 0x18), iVar1 != 1)) {
        iVar1 = _hit(p->x + 8,p->y + p->height);
        if (iVar1 == 1) {
          iVar1 = p->y;
          if (iVar1 < 0) {
            iVar1 = iVar1 + 0x1f;
          }
          p->y = ((iVar1 >> 5) * 0x20 + 0x20) - p->height;
        }
        if (((p->state != 2) && (p->state != 4)) && ((p->state != 1 && (p->state != 3)))) {
          p->state = 2;
          p->frame = 0xd2;
        }
        if (0 < p->x) {
          p->frame = p->frame + 1;
          if (p->state == 4) {
            if (0x67 < p->frame) {
              p->frame = 0x4a;
            }
          }
          else if (0xef < p->frame) {
            p->frame = 0xd2;
          }
          p->x = p->x + -2;
          p->vx = -3;
        }
      }
    }
  }
  return;
}

