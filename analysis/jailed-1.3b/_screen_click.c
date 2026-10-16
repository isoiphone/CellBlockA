/* JAILED.EXE address: 00001b0c */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _screen_click(void)

{
  byte *pbVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int local_14;
  int local_8;
  
  iVar2 = _mouse_x;
  if (_mouse_x < 0) {
    iVar2 = _mouse_x + 0x1f;
  }
  iVar2 = iVar2 >> 5;
  iVar3 = _mouse_y;
  if (_mouse_y < 0) {
    iVar3 = _mouse_y + 0x1f;
  }
  iVar3 = iVar3 >> 5;
  if (iVar3 < 0xd) {
    switch(_CUR_EDIT) {
    case 1:
      if ((_mouse_b & 2) == 0) {
        iVar4 = _rand();
        local_14 = iVar4;
        if (iVar4 < 0) {
          local_14 = iVar4 + 3;
        }
        _map[iVar2][iVar3].kind = (char)iVar4 + (char)(local_14 >> 2) * -4 + '\x06';
      }
      else if ((5 < _map[iVar2][iVar3].kind) && (_map[iVar2][iVar3].kind < 0x12)) {
        _map[iVar2][iVar3].kind = '\0';
      }
      break;
    case 2:
      if ((_mouse_b & 2) == 0) {
        iVar4 = _rand();
        local_14 = iVar4;
        if (iVar4 < 0) {
          local_14 = iVar4 + 3;
        }
        _map[iVar2][iVar3].kind = (char)iVar4 + (char)(local_14 >> 2) * -4 + '\n';
      }
      else if ((5 < _map[iVar2][iVar3].kind) && (_map[iVar2][iVar3].kind < 0x12)) {
        _map[iVar2][iVar3].kind = '\0';
      }
      break;
    case 3:
      if ((_mouse_b & 2) == 0) {
        iVar4 = _rand();
        local_14 = iVar4;
        if (iVar4 < 0) {
          local_14 = iVar4 + 3;
        }
        _map[iVar2][iVar3].kind = (char)iVar4 + (char)(local_14 >> 2) * -4 + '\x0e';
      }
      else if ((5 < _map[iVar2][iVar3].kind) && (_map[iVar2][iVar3].kind < 0x12)) {
        _map[iVar2][iVar3].kind = '\0';
      }
      break;
    case 4:
      if ((_mouse_b & 2) == 0) {
        _map[iVar2][iVar3].kind = '\x04';
      }
      else if (_map[iVar2][iVar3].kind == '\x04') {
        _map[iVar2][iVar3].kind = '\0';
      }
      break;
    case 5:
      if ((_mouse_b & 2) == 0) {
        for (local_8 = 0; local_8 < 4; local_8 = local_8 + 1) {
          iVar2 = _mouse_x;
          if (_mouse_x < 0) {
            iVar2 = _mouse_x + 0x1f;
          }
          if ((int)((char)((char)_start[local_8].packed * '\x02') >> 1) == iVar2 >> 5) {
            iVar2 = _mouse_y;
            if (_mouse_y < 0) {
              iVar2 = _mouse_y + 0x1f;
            }
            if ((int)((short)(_start[local_8].packed << 2) >> 9) == iVar2 >> 5) {
              if ((*(byte *)((int)&_start[local_8].packed + 1) >> 6 & 1) == 1) {
                pbVar1 = (byte *)((int)&_start[local_8].packed + 1);
                *pbVar1 = *pbVar1 & 0xbf;
              }
              else {
                pbVar1 = (byte *)((int)&_start[local_8].packed + 1);
                *pbVar1 = *pbVar1 | 0x40;
              }
              _mouse_b = 0;
              iVar2 = _mouse_y;
              if (_mouse_y < 0) {
                iVar2 = _mouse_y + 0x1f;
              }
              iVar3 = _mouse_x;
              if (_mouse_x < 0) {
                iVar3 = _mouse_x + 0x1f;
              }
              _draw_tile(_buffer,iVar3 >> 5,iVar2 >> 5);
              return;
            }
          }
        }
        for (local_8 = 0; local_8 < 4; local_8 = local_8 + 1) {
          if (((char)((char)_start[local_8].packed * '\x02') >> 1 == -1) &&
             ((short)(_start[local_8].packed << 2) >> 9 == -1)) {
            iVar2 = _mouse_x;
            if (_mouse_x < 0) {
              iVar2 = _mouse_x + 0x1f;
            }
            *(byte *)&_start[local_8].packed = (byte)_start[local_8].packed & 0x80;
            *(byte *)&_start[local_8].packed =
                 (byte)_start[local_8].packed | (byte)(iVar2 >> 5) & 0x7f;
            iVar2 = _mouse_y;
            if (_mouse_y < 0) {
              iVar2 = _mouse_y + 0x1f;
            }
            _start[local_8].packed = _start[local_8].packed & 0xc07f;
            _start[local_8].packed = _start[local_8].packed | ((ushort)(iVar2 >> 5) & 0x7f) << 7;
            pbVar1 = (byte *)((int)&_start[local_8].packed + 1);
            *pbVar1 = *pbVar1 & 0xbf;
            break;
          }
        }
      }
      else {
        for (local_8 = 0; local_8 < 4; local_8 = local_8 + 1) {
          iVar2 = _mouse_x;
          if (_mouse_x < 0) {
            iVar2 = _mouse_x + 0x1f;
          }
          if ((int)((char)((char)_start[local_8].packed * '\x02') >> 1) == iVar2 >> 5) {
            iVar2 = _mouse_y;
            if (_mouse_y < 0) {
              iVar2 = _mouse_y + 0x1f;
            }
            if ((int)((short)(_start[local_8].packed << 2) >> 9) == iVar2 >> 5) {
              *(byte *)&_start[local_8].packed = (byte)_start[local_8].packed | 0x7f;
              _start[local_8].packed = _start[local_8].packed | 0x3f80;
              break;
            }
          }
        }
      }
    }
    _mouse_b = 0;
    iVar2 = _mouse_y;
    if (_mouse_y < 0) {
      iVar2 = _mouse_y + 0x1f;
    }
    iVar3 = _mouse_x;
    if (_mouse_x < 0) {
      iVar3 = _mouse_x + 0x1f;
    }
    _draw_tile(_buffer,iVar3 >> 5,iVar2 >> 5);
  }
  return;
}

