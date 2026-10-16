/* Original CELL.EXE address: 0000b76c */

void _get_keys(void)

{
  int iVar1;
  int iVar2;
  BITMAP *dst;
  bool bVar3;
  char local_48 [64];
  int local_8;
  
  _player[0].up = 0;
  _player[0].down = 0;
  _player[0].left = 0;
  _player[0].right = 0;
  _player[1].up = 0;
  _player[1].down = 0;
  _player[1].left = 0;
  _player[1].right = 0;
  if (_key[0x1e] == '\0') {
    if (_key[0x20] == '\0') {
      if (_player[0].state == 2) {
        _player[0].state = 0;
        _player[0].frame = 0x138;
      }
    }
    else {
      _player[0].right = 1;
    }
  }
  else {
    _player[0].left = 1;
  }
  if (_key[0x11] == '\0') {
    if (_key[0x1f] != '\0') {
      _player[0].down = 1;
    }
  }
  else {
    _player[0].up = 1;
  }
  if (_key[0x10] != '\0') {
    _player_attack(0);
  }
  if (_key[0x4b] == '\0') {
    if (_key[0x4d] == '\0') {
      if (_player[1].state == 2) {
        _player[1].state = 0;
        _player[1].frame = 0x138;
      }
    }
    else {
      _player[1].right = 1;
    }
  }
  else {
    _player[1].left = 1;
  }
  if (_key[0x48] == '\0') {
    if (_key[0x50] != '\0') {
      _player[1].down = 1;
    }
  }
  else {
    _player[1].up = 1;
  }
  if (_key[0x78] != '\0') {
    _player_attack(1);
  }
  if (_key[1] == '\0') {
    if (_key[0x58] == '\0') {
      if (_key[0x54] == '\0') {
        if (_key[0x2d] == '\0') {
          if (_key[0x45] == '\0') {
            if (_key[0x7a] == '\0') {
              if (_key[0x37] == '\0') {
                if (_key[0x4a] == '\0') {
                  if (_key[0x4e] != '\0') {
                    iVar2 = _is_mod_playing();
                    if (iVar2 == 0) {
                      _play_mod(_music,-1);
                    }
                    else {
                      _stop_mod();
                    }
                    _key[0x4e] = '\0';
                  }
                }
                else {
                  iVar2 = _is_mod_paused();
                  if (iVar2 == 0) {
                    _pause_mod();
                  }
                  else {
                    _resume_mod();
                  }
                  _key[0x4a] = '\0';
                }
              }
              else {
                _next_song();
                _key[0x37] = '\0';
              }
            }
            else {
              _prev_song();
              _key[0x7a] = '\0';
            }
          }
          else {
            _rand_song();
            _key[0x45] = '\0';
          }
        }
      }
      else {
        local_8 = 1;
        _sprintf(local_48,"shot%i.pcx",1);
        while ((iVar1 = _exists(local_48), iVar2 = local_8, iVar1 != 0 && (local_8 < 100))) {
          local_8 = local_8 + 1;
          _sprintf(local_48,"shot%i.pcx",iVar2);
        }
        if (local_8 < 100) {
          if (_gfx_driver == (GFX_DRIVER *)0x0) {
            iVar2 = 0;
          }
          else {
            iVar2 = _gfx_driver->w / 2;
          }
          _play_sound(0x161,iVar2);
          dst = _create_bitmap(0x280,0x1e0);
          _blit(_screen,dst,0,0,0,0,0x280,0x1e0);
          _save_pcx(local_48,dst,(RGB *)&__current_pallete);
          _destroy_bitmap(dst);
        }
        _key[0x54] = '\0';
      }
    }
    else {
      bVar3 = _show_fps == 0;
      if (!bVar3) {
        _draw_tile(_buffer,0x12,0);
        _draw_tile(_buffer,0x13,0);
      }
      _show_fps = (uint)bVar3;
      _key[0x58] = '\0';
    }
  }
  else {
    _going = 0;
  }
  return;
}

