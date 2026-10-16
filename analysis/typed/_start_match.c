/* Original CELL.EXE address: 00007cc4 */

void _start_match(void)

{
  int iVar1;
  
  _endmatch = 0;
  _match = _match + 1;
  iVar1 = _clock();
  _match_start = iVar1 + -1;
  _clear(_buffer);
  _load_map(_cur_map);
  _draw_map();
  _draw_map();
  _reset_points();
  _spawn_player(0);
  _spawn_player(1);
  _install_int_ex(_item_timer,0x1234dd);
  _install_int_ex(_fps_timer,0x1234dd);
  _install_int(_game_tick,0x14);
  _install_int(_translate_points,10);
  return;
}

