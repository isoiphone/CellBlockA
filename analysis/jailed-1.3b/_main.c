/* JAILED.EXE address: 000023bc */

int _main(int argc,char **argv)

{
  int iVar1;
  int w;
  undefined4 local_150;
  undefined2 local_14c;
  undefined1 local_14a [122];
  undefined4 local_d0;
  undefined2 local_cc;
  undefined1 local_ca [122];
  char local_50 [72];
  int local_8;
  
  _allegro_init();
  _install_keyboard();
  iVar1 = _set_gfx_mode(0,0x280,0x1e0,0,0);
  if (iVar1 < 0) {
    _error(1,"setting 640x480x256!");
  }
  _install_timer();
  _game_data = _load_datafile("game.dat");
  if (_game_data == (DATAFILE *)0x0) {
    _error(3,"loading game.dat!");
  }
  _set_pallete(_game_data[10].dat);
  iVar1 = _install_mouse();
  if (iVar1 < 0) {
    _error(2,"loading mouse!");
  }
  _set_mouse_sprite(_game_data[2].dat);
  _set_mouse_sprite_focus(0,0);
  _buffer = _create_bitmap(0x280,0x1e0);
  _clear(_buffer);
  _reset_map();
  _draw_map();
  do {
    if (((_mouse_b & 1) != 0) || ((_mouse_b & 2) != 0)) {
      _screen_click();
    }
    if (_key[1] == '\0') {
      if (_key[0x1f] == '\0') {
        if (_key[0x26] == '\0') {
          if (_key[0x31] == '\0') {
            if (_key[0x10] == '\0') {
              if (_key[0x11] == '\0') {
                if (_key[0x12] == '\0') {
                  if (_key[0x13] == '\0') {
                    if (_key[0x14] != '\0') {
                      _set_mouse_sprite(_game_data[5].dat);
                      _CUR_EDIT = 5;
                    }
                  }
                  else {
                    _CUR_EDIT = 3;
                    _set_mouse_sprite(_game_data[4].dat);
                  }
                }
                else {
                  _CUR_EDIT = 2;
                  _set_mouse_sprite(_game_data[3].dat);
                }
              }
              else {
                _CUR_EDIT = 1;
                _set_mouse_sprite(_game_data[2].dat);
              }
            }
            else {
              _CUR_EDIT = 4;
              _set_mouse_sprite(_game_data[1].dat);
            }
          }
          else {
            _reset_map();
            _draw_map();
          }
        }
        else {
          local_150 = DAT_0000239d;
          local_14c = DAT_000023a1;
          _memset(local_14a,0,0x7a);
          iVar1 = _file_select("Load...",&local_150,&DAT_000023a3);
          if (iVar1 != 0) {
            _load_map((char *)&local_150);
            _draw_map();
          }
          _text_mode(0xffffffff);
        }
      }
      else {
        local_d0 = DAT_0000239d;
        local_cc = DAT_000023a1;
        _memset(local_ca,0,0x7a);
        iVar1 = _file_select("Save As...",&local_d0,&DAT_000023a3);
        if (iVar1 != 0) {
          _save_map((char *)&local_d0);
          _draw_map();
        }
        _text_mode(0xffffffff);
      }
    }
    else {
      iVar1 = _alert(&DAT_00002363,"Do you really want to quit?",0,&DAT_00002343,&DAT_00002340,0,
                     0xff);
      if (iVar1 == 1) {
        _sprintf(local_50,"Jail-ed (Cell Block A Map Editor) - Version %s\n",&DAT_00002368);
        _error(0,local_50);
        return 0;
      }
    }
    _freeze_mouse_flag = 1;
    local_8 = _mouse_y;
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      iVar1 = 0;
    }
    else {
      iVar1 = _gfx_driver->w;
    }
    _blit(_buffer,_screen,0,0,0,0,iVar1,_mouse_y);
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      iVar1 = -local_8;
    }
    else {
      iVar1 = _gfx_driver->h - local_8;
    }
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      w = 0;
    }
    else {
      w = _gfx_driver->w;
    }
    _blit(_buffer,_screen,0,local_8 + 0x10,0,local_8 + 0x10,w,iVar1 + -0x10);
    _vsync();
    _show_mouse(0);
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      iVar1 = 0;
    }
    else {
      iVar1 = _gfx_driver->w;
    }
    _blit(_buffer,_screen,0,local_8,0,local_8,iVar1,0x10);
    _show_mouse(_screen);
    _freeze_mouse_flag = 0;
  } while( true );
}

