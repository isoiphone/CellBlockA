/* Original CELL.EXE address: 00005c68 */

undefined4 _main_menu(void)

{
  int iVar1;
  int iVar2;
  
  _menu_data = _load_datafile("menu.dat");
  if (_menu_data == (DATAFILE *)0x0) {
    _error(3,"loading menu.dat!");
  }
  _start_music();
  _clear(_buffer);
  _install_int_ex(_update_nail,0x1234d);
  _selector.frame = 0;
  _selector.y = 0;
  _selector.tick = 0;
  _selector.active = 0;
  _fade_out(3);
  if (_gfx_driver == (GFX_DRIVER *)0x0) {
    iVar1 = 0;
    iVar2 = 0;
  }
  else {
    iVar1 = _gfx_driver->h;
    iVar2 = _gfx_driver->w;
  }
  _blit(_menu_data[3].dat,_screen,0,0,0,0,iVar2,iVar1);
  _fade_in(_menu_data[0x14].dat,3);
  _set_pallete(_menu_data[0x14].dat);
  do {
    while( true ) {
      while( true ) {
        if (_playing == 0) {
          _set_pallete(_menu_data[0x14].dat);
          _stop_mod();
          _fade_out(3);
          _remove_int(_update_nail);
          _unload_datafile(_menu_data);
          return 0;
        }
        _clear(_buffer);
        if (_selector.active == 1) break;
        if (_key[0x48] == '\0') {
          if (_key[0x50] == '\0') {
            if (_key[0x1c] != '\0') {
              _selector.tick = 0;
              _selector.active = 1;
              _key[0x1c] = '\0';
            }
          }
          else {
            if (_selector.y == 0) {
              _selector.y = 0x30;
            }
            else if (_selector.y == 0x30) {
              _selector.y = 0x61;
            }
            else if (_selector.y == 0x61) {
              _selector.y = 0x90;
            }
            else if (_selector.y == 0x90) {
              _selector.y = 0;
            }
            _key[0x50] = '\0';
          }
        }
        else {
          if (_selector.y == 0) {
            _selector.y = 0x90;
          }
          else if (_selector.y == 0x30) {
            _selector.y = 0;
          }
          else if (_selector.y == 0x61) {
            _selector.y = 0x30;
          }
          else if (_selector.y == 0x90) {
            _selector.y = 0x61;
          }
          _key[0x48] = '\0';
        }
        if (_gfx_driver == (GFX_DRIVER *)0x0) {
          iVar1 = 0;
          iVar2 = 0;
        }
        else {
          iVar1 = _gfx_driver->h;
          iVar2 = _gfx_driver->w;
        }
        _blit(_menu_data[3].dat,_buffer,0,0,0,0,iVar2,iVar1);
        _draw_nail();
        _vsync();
        if (_gfx_driver == (GFX_DRIVER *)0x0) {
          iVar1 = 0;
          iVar2 = 0;
        }
        else {
          iVar1 = _gfx_driver->h;
          iVar2 = _gfx_driver->w;
        }
        _blit(_buffer,_screen,0,0,0,0,iVar2,iVar1);
      }
      if (_selector.y != 0) break;
      _play_menu();
L513:
      _selector.tick = 0;
      _selector.active = 0;
      if (_gfx_driver == (GFX_DRIVER *)0x0) {
        iVar1 = 0;
        iVar2 = 0;
      }
      else {
        iVar1 = _gfx_driver->h;
        iVar2 = _gfx_driver->w;
      }
      _blit(_menu_data[3].dat,_screen,0,0,0,0,iVar2,iVar1);
      _fade_in(_menu_data[0x14].dat,3);
      _set_pallete(_menu_data[0x14].dat);
    }
    if (_selector.y == 0x30) {
      _help_menu();
      goto L513;
    }
    if (_selector.y == 0x61) {
      _options_menu();
      goto L513;
    }
    if (_selector.y != 0x90) goto L513;
    _playing = 0;
  } while( true );
}

