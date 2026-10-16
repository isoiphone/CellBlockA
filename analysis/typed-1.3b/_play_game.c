/* Original 1.3b CELL.EXE address: 0000b970 */

int _play_game(char *filename)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int local_18;
  int local_c;
  
  _going = 1;
  _player1_wins = 0;
  _player2_wins = 0;
  __go32_dpmi_lock_data(&_last_fps,4);
  __go32_dpmi_lock_data(&_frame_counter,4);
  _reset_points();
  _cur_map = filename;
  _load_map(filename);
  _blit(_game_data->dat,native_arena_background,0,0,0,0,0x280,0x1e0);
  local_c = _rand();
  for (local_c = local_c % 3; 0 < local_c; local_c = local_c + -1) {
    uVar1 = _rand();
    if ((uVar1 & 1) == 0) {
      iVar2 = _rand();
      iVar4 = iVar2 % 0x14 + 400;
      iVar2 = _rand();
      iVar2 = iVar2 % 0x280;
      iVar3 = _rand();
      _draw_sprite_h_flip(native_arena_background,_game_data[iVar3 % 3 + 0x68].dat,iVar2,iVar4);
    }
    else {
      iVar2 = _rand();
      iVar4 = iVar2 % 0x14 + 400;
      iVar2 = _rand();
      iVar2 = iVar2 % 0x280;
      iVar3 = _rand();
      _draw_sprite(native_arena_background,_game_data[iVar3 % 3 + 0x68].dat,iVar2,iVar4);
    }
  }
  _set_pallete(_game_data[10].dat);
  _draw_map();
  _spawn_player(0);
  _spawn_player(1);
  _install_int_ex(_item_timer,0x1234dd);
  _install_int_ex(_fps_timer,0x1234dd);
  _install_int(_game_tick,0x14);
  _install_int(_translate_points,10);
  _match_start = _clock();
  _match = 1;
  while (_going != 0) {
    if ((((_player[0].health < '\x01') || (_player[1].health < '\x01')) && (_endmatch < 1)) ||
       (_endmatch == -1)) {
      _end_match();
      _start_match();
    }
    _get_keys();
    _clean_map();
    if (_show_fps != 0) {
      _textprintf(_buffer,_game_data[7].dat,0x1d,0x1c5,-1,"%i",_last_fps);
    }
    _textprintf(_buffer,_game_data[7].dat,8,8,-1,"%2d",(int)_player[0].health);
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      local_18 = -0x1e;
    }
    else {
      local_18 = _gfx_driver->w + -0x1e;
    }
    _textprintf(_buffer,_game_data[7].dat,local_18,8,-1,"%2d",(int)_player[1].health);
    _rectfill(_buffer,0xc1,0xf,0x1d1,0x19,0);
    _textprintf_centre(_buffer,_game_data[9].dat,0x149,0xf,-1,"%s",_music);
    if (_player[1].state == 6) {
      _draw_player(0);
      _draw_player(1);
    }
    else {
      _draw_player(1);
      _draw_player(0);
    }
    _draw_points();
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      local_18 = 0;
      iVar2 = local_18;
      local_18 = 0;
    }
    else {
      iVar2 = _gfx_driver->h;
      local_18 = _gfx_driver->w;
    }
    _blit(_buffer,_screen,0,0,0,0,local_18,iVar2);
    _frame_counter = _frame_counter + 1;
  }
  _remove_int(_item_timer);
  _remove_int(_fps_timer);
  _remove_int(_game_tick);
  _remove_int(_translate_points);
  return 0;
}

