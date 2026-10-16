/* Original 1.3b CELL.EXE address: 00005c48 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _options_menu(void)

{
  double dVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = _selector.y;
  iVar2 = _selector.frame;
  _selector.frame = 5;
  _selector.y = 0x5c;
  _selector.tick = 0;
  _selector.active = 0;
  _fade_out(3);
  if (_gfx_driver == (GFX_DRIVER *)0x0) {
    iVar4 = 0;
    iVar5 = 0;
  }
  else {
    iVar4 = _gfx_driver->h;
    iVar5 = _gfx_driver->w;
  }
  _blit(_menu_data[6].dat,_screen,0,0,0,0,iVar5,iVar4);
  _fade_in(_menu_data[0x18].dat,3);
  _set_pallete(_menu_data[0x18].dat);
  while (_key[1] == '\0') {
    _clear(_buffer);
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      iVar4 = 0;
      iVar5 = 0;
    }
    else {
      iVar4 = _gfx_driver->h;
      iVar5 = _gfx_driver->w;
    }
    _blit(_menu_data[6].dat,_buffer,0,0,0,0,iVar5,iVar4);
    if (_key[0x48] == '\0') {
      if (_key[0x50] == '\0') {
        if (_key[0x4b] == '\0') {
          if (_key[0x4d] != '\0') {
            if (_selector.y == 0x5c) {
              if ((_nosound == 0) && (_game_config.sound < 100)) {
                _game_config.sound = _game_config.sound + 1;
              }
            }
            else if (_selector.y == 0x9f) {
              if ((_nosound == 0) && (_game_config.music < 100)) {
                dVar1 = (double)_game_config.music;
                _game_config.music = _game_config.music + 1;
                _set_mod_volume((int)ROUND(_DAT_00005c40 * dVar1));
              }
            }
            else if (_selector.y == 0xe1) {
              if (_game_config.maxkills < 999) {
                _game_config.maxkills = _game_config.maxkills + 1;
              }
            }
            else if ((_selector.y == 0x11d) && (_game_config.maxtime < 999)) {
              _game_config.maxtime = _game_config.maxtime + 1;
            }
            _key[0x4d] = '\0';
          }
        }
        else {
          if (_selector.y == 0x5c) {
            if ((_nosound == 0) && (0 < _game_config.sound)) {
              _game_config.sound = _game_config.sound + -1;
            }
          }
          else if (_selector.y == 0x9f) {
            if ((_nosound == 0) && (0 < _game_config.music)) {
              dVar1 = (double)_game_config.music;
              _game_config.music = _game_config.music + -1;
              _set_mod_volume((int)ROUND(_DAT_00005c40 * dVar1));
            }
          }
          else if (_selector.y == 0xe1) {
            if (0 < _game_config.maxkills) {
              _game_config.maxkills = _game_config.maxkills + -1;
            }
          }
          else if ((_selector.y == 0x11d) && (0 < _game_config.maxtime)) {
            _game_config.maxtime = _game_config.maxtime + -1;
          }
          _key[0x4b] = '\0';
        }
      }
      else {
        if (_selector.y == 0x5c) {
          _selector.y = 0x9f;
        }
        else if (_selector.y == 0x9f) {
          _selector.y = 0xe1;
        }
        else if (_selector.y == 0xe1) {
          _selector.y = 0x11d;
        }
        else if (_selector.y == 0x11d) {
          _selector.y = 0x5c;
        }
        _key[0x50] = '\0';
      }
    }
    else {
      if (_selector.y == 0x5c) {
        _selector.y = 0x11d;
      }
      else if (_selector.y == 0x9f) {
        _selector.y = 0x5c;
      }
      else if (_selector.y == 0xe1) {
        _selector.y = 0x9f;
      }
      else if (_selector.y == 0x11d) {
        _selector.y = 0xe1;
      }
      _key[0x48] = '\0';
    }
    if (_nosound == 0) {
      _draw_sprite(_buffer,_menu_data[0x12].dat,_game_config.sound + 0x43,0x66);
      _draw_sprite(_buffer,_menu_data[0x12].dat,_game_config.music + 0x43,0xa7);
    }
    _textprintf(_buffer,_game_data[6].dat,0x49,0xf2,0xd6,"%3d",_game_config.maxkills);
    _textprintf(_buffer,_game_data[6].dat,0x4a,0x12e,0xd6,"%3d",_game_config.maxtime);
    _draw_nail();
    _vsync();
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      iVar4 = 0;
      iVar5 = 0;
    }
    else {
      iVar4 = _gfx_driver->h;
      iVar5 = _gfx_driver->w;
    }
    _blit(_buffer,_screen,0,0,0,0,iVar5,iVar4);
  }
  _selector.frame = iVar2;
  _selector.y = iVar3;
  _fade_out(3);
  return;
}

