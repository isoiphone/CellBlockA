/* JAILED.EXE address: 000023b0 */

undefined4 _main(void)

{
  int iVar1;
  undefined4 uVar2;
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
  if (_game_data == 0) {
    _error(3,"loading game.dat!");
  }
  _set_pallete(*(undefined4 *)(_game_data + 0xa0));
  iVar1 = _install_mouse();
  if (iVar1 < 0) {
    _error(2,"loading mouse!");
  }
  _set_mouse_sprite(*(undefined4 *)(_game_data + 0x20));
  _set_mouse_sprite_focus(0,0);
  _buffer = _create_bitmap(0x280,0x1e0);
  _clear(_buffer);
  _reset_map();
  _draw_map();
  do {
    if (((_mouse_b & 1) != 0) || ((_mouse_b & 2) != 0)) {
      _screen_click();
    }
    if (DAT_0007c261 == '\0') {
      if (DAT_0007c27f == '\0') {
        if (DAT_0007c286 == '\0') {
          if (DAT_0007c291 == '\0') {
            if (DAT_0007c270 == '\0') {
              if (DAT_0007c271 == '\0') {
                if (DAT_0007c272 == '\0') {
                  if (DAT_0007c273 == '\0') {
                    if (DAT_0007c274 != '\0') {
                      _set_mouse_sprite(*(undefined4 *)(_game_data + 0x50));
                      _CUR_EDIT = 5;
                    }
                  }
                  else {
                    _CUR_EDIT = 3;
                    _set_mouse_sprite(*(undefined4 *)(_game_data + 0x40));
                  }
                }
                else {
                  _CUR_EDIT = 2;
                  _set_mouse_sprite(*(undefined4 *)(_game_data + 0x30));
                }
              }
              else {
                _CUR_EDIT = 1;
                _set_mouse_sprite(*(undefined4 *)(_game_data + 0x20));
              }
            }
            else {
              _CUR_EDIT = 4;
              _set_mouse_sprite(*(undefined4 *)(_game_data + 0x10));
            }
          }
          else {
            _reset_map();
            _draw_map();
          }
        }
        else {
          local_150 = LC21;
          local_14c = DAT_00002394;
          _memset(local_14a,0,0x7a);
          iVar1 = _file_select("Load...",&local_150,&LC22);
          if (iVar1 != 0) {
            _load_map(&local_150);
            _draw_map();
          }
          _text_mode(0xffffffff);
        }
      }
      else {
        local_d0 = LC21;
        local_cc = DAT_00002394;
        _memset(local_ca,0,0x7a);
        iVar1 = _file_select("Save As...",&local_d0,&LC22);
        if (iVar1 != 0) {
          _save_map(&local_d0);
          _draw_map();
        }
        _text_mode(0xffffffff);
      }
    }
    else {
      iVar1 = _alert(&LC18,"Do you really want to quit?",0,&LC16,&LC15,0,0xff);
      if (iVar1 == 1) {
        _sprintf(local_50,"Jail-ed (Cell Block A Map Editor) - Version %s\n",&LC19);
        _error(0,local_50);
        return 0;
      }
    }
    _freeze_mouse_flag = 1;
    local_8 = _mouse_y;
    if (_gfx_driver == 0) {
      uVar2 = 0;
    }
    else {
      uVar2 = *(undefined4 *)(_gfx_driver + 0x1c);
    }
    _blit(_buffer,_screen,0,0,0,0,uVar2,_mouse_y);
    if (_gfx_driver == 0) {
      iVar1 = -local_8;
    }
    else {
      iVar1 = *(int *)(_gfx_driver + 0x20) - local_8;
    }
    if (_gfx_driver == 0) {
      uVar2 = 0;
    }
    else {
      uVar2 = *(undefined4 *)(_gfx_driver + 0x1c);
    }
    _blit(_buffer,_screen,0,local_8 + 0x10,0,local_8 + 0x10,uVar2,iVar1 + -0x10);
    _vsync();
    _show_mouse(0);
    if (_gfx_driver == 0) {
      uVar2 = 0;
    }
    else {
      uVar2 = *(undefined4 *)(_gfx_driver + 0x1c);
    }
    _blit(_buffer,_screen,0,local_8,0,local_8,uVar2,0x10);
    _show_mouse(_screen);
    _freeze_mouse_flag = 0;
  } while( true );
}

