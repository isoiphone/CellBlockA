/* JAILED.EXE address: 00001b00 */

void _screen_click(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_14;
  int local_8;
  
  iVar1 = _mouse_x;
  if (_mouse_x < 0) {
    iVar1 = _mouse_x + 0x1f;
  }
  iVar1 = iVar1 >> 5;
  iVar2 = _mouse_y;
  if (_mouse_y < 0) {
    iVar2 = _mouse_y + 0x1f;
  }
  iVar2 = iVar2 >> 5;
  if (iVar2 < 0xd) {
    switch(_CUR_EDIT) {
    case 1:
      if ((_mouse_b & 2) == 0) {
        iVar3 = _rand();
        local_14 = iVar3;
        if (iVar3 < 0) {
          local_14 = iVar3 + 3;
        }
        (&_map)[iVar2 * 2 + iVar1 * 0x1a] = (char)iVar3 + (char)(local_14 >> 2) * -4 + '\x06';
      }
      else if ((5 < (byte)(&_map)[iVar2 * 2 + iVar1 * 0x1a]) &&
              ((byte)(&_map)[iVar2 * 2 + iVar1 * 0x1a] < 0x12)) {
        (&_map)[iVar2 * 2 + iVar1 * 0x1a] = 0;
      }
      break;
    case 2:
      if ((_mouse_b & 2) == 0) {
        iVar3 = _rand();
        local_14 = iVar3;
        if (iVar3 < 0) {
          local_14 = iVar3 + 3;
        }
        (&_map)[iVar2 * 2 + iVar1 * 0x1a] = (char)iVar3 + (char)(local_14 >> 2) * -4 + '\n';
      }
      else if ((5 < (byte)(&_map)[iVar2 * 2 + iVar1 * 0x1a]) &&
              ((byte)(&_map)[iVar2 * 2 + iVar1 * 0x1a] < 0x12)) {
        (&_map)[iVar2 * 2 + iVar1 * 0x1a] = 0;
      }
      break;
    case 3:
      if ((_mouse_b & 2) == 0) {
        iVar3 = _rand();
        local_14 = iVar3;
        if (iVar3 < 0) {
          local_14 = iVar3 + 3;
        }
        (&_map)[iVar2 * 2 + iVar1 * 0x1a] = (char)iVar3 + (char)(local_14 >> 2) * -4 + '\x0e';
      }
      else if ((5 < (byte)(&_map)[iVar2 * 2 + iVar1 * 0x1a]) &&
              ((byte)(&_map)[iVar2 * 2 + iVar1 * 0x1a] < 0x12)) {
        (&_map)[iVar2 * 2 + iVar1 * 0x1a] = 0;
      }
      break;
    case 4:
      if ((_mouse_b & 2) == 0) {
        (&_map)[iVar2 * 2 + iVar1 * 0x1a] = 4;
      }
      else if ((&_map)[iVar2 * 2 + iVar1 * 0x1a] == '\x04') {
        (&_map)[iVar2 * 2 + iVar1 * 0x1a] = 0;
      }
      break;
    case 5:
      if ((_mouse_b & 2) == 0) {
        for (local_8 = 0; local_8 < 4; local_8 = local_8 + 1) {
          iVar1 = _mouse_x;
          if (_mouse_x < 0) {
            iVar1 = _mouse_x + 0x1f;
          }
          if ((int)((char)((&_start)[local_8 * 4] * '\x02') >> 1) == iVar1 >> 5) {
            iVar1 = _mouse_y;
            if (_mouse_y < 0) {
              iVar1 = _mouse_y + 0x1f;
            }
            if ((int)((short)(*(short *)(&_start + local_8 * 4) << 2) >> 9) == iVar1 >> 5) {
              if (((byte)(&DAT_00079c85)[local_8 * 4] >> 6 & 1) == 1) {
                (&DAT_00079c85)[local_8 * 4] = (&DAT_00079c85)[local_8 * 4] & 0xbf;
              }
              else {
                (&DAT_00079c85)[local_8 * 4] = (&DAT_00079c85)[local_8 * 4] | 0x40;
              }
              _mouse_b = 0;
              iVar1 = _mouse_y;
              if (_mouse_y < 0) {
                iVar1 = _mouse_y + 0x1f;
              }
              iVar2 = _mouse_x;
              if (_mouse_x < 0) {
                iVar2 = _mouse_x + 0x1f;
              }
              _draw_tile(_buffer,iVar2 >> 5,iVar1 >> 5);
              return;
            }
          }
        }
        for (local_8 = 0; local_8 < 4; local_8 = local_8 + 1) {
          if (((char)((&_start)[local_8 * 4] * '\x02') >> 1 == -1) &&
             ((short)(*(short *)(&_start + local_8 * 4) << 2) >> 9 == -1)) {
            iVar1 = _mouse_x;
            if (_mouse_x < 0) {
              iVar1 = _mouse_x + 0x1f;
            }
            (&_start)[local_8 * 4] = (&_start)[local_8 * 4] & 0x80;
            (&_start)[local_8 * 4] = (&_start)[local_8 * 4] | (byte)(iVar1 >> 5) & 0x7f;
            iVar1 = _mouse_y;
            if (_mouse_y < 0) {
              iVar1 = _mouse_y + 0x1f;
            }
            *(ushort *)(&_start + local_8 * 4) = *(ushort *)(&_start + local_8 * 4) & 0xc07f;
            *(ushort *)(&_start + local_8 * 4) =
                 *(ushort *)(&_start + local_8 * 4) | ((ushort)(iVar1 >> 5) & 0x7f) << 7;
            (&DAT_00079c85)[local_8 * 4] = (&DAT_00079c85)[local_8 * 4] & 0xbf;
            break;
          }
        }
      }
      else {
        for (local_8 = 0; local_8 < 4; local_8 = local_8 + 1) {
          iVar1 = _mouse_x;
          if (_mouse_x < 0) {
            iVar1 = _mouse_x + 0x1f;
          }
          if ((int)((char)((&_start)[local_8 * 4] * '\x02') >> 1) == iVar1 >> 5) {
            iVar1 = _mouse_y;
            if (_mouse_y < 0) {
              iVar1 = _mouse_y + 0x1f;
            }
            if ((int)((short)(*(short *)(&_start + local_8 * 4) << 2) >> 9) == iVar1 >> 5) {
              (&_start)[local_8 * 4] = (&_start)[local_8 * 4] | 0x7f;
              *(ushort *)(&_start + local_8 * 4) = *(ushort *)(&_start + local_8 * 4) | 0x3f80;
              break;
            }
          }
        }
      }
    }
    _mouse_b = 0;
    iVar1 = _mouse_y;
    if (_mouse_y < 0) {
      iVar1 = _mouse_y + 0x1f;
    }
    iVar2 = _mouse_x;
    if (_mouse_x < 0) {
      iVar2 = _mouse_x + 0x1f;
    }
    _draw_tile(_buffer,iVar2 >> 5,iVar1 >> 5);
  }
  return;
}

