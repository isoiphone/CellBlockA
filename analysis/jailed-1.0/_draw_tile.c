/* JAILED.EXE address: 00002b98 */

void _draw_tile(undefined4 param_1,int param_2,int param_3)

{
  int local_8;
  
  _blit(*_game_data,param_1,param_2 << 5,param_3 << 5,param_2 << 5,param_3 << 5,0x20,0x20);
  switch((&_map)[param_3 * 2 + param_2 * 0x1a]) {
  case 4:
    _draw_sprite(param_1,_game_data[0x5ac],param_2 << 5,param_3 << 5);
    break;
  case 6:
    _draw_sprite(param_1,_game_data[0x5b0],param_2 << 5,param_3 << 5);
    break;
  case 7:
    _draw_sprite(param_1,_game_data[0x5b4],param_2 << 5,param_3 << 5);
    break;
  case 8:
    _draw_sprite(param_1,_game_data[0x5b8],param_2 << 5,param_3 << 5);
    break;
  case 9:
    _draw_sprite(param_1,_game_data[0x5bc],param_2 << 5,param_3 << 5);
    break;
  case 10:
    _draw_sprite(param_1,_game_data[0x5c0],param_2 << 5,param_3 << 5);
    break;
  case 0xb:
    _draw_sprite(param_1,_game_data[0x5c4],param_2 << 5,param_3 << 5);
    break;
  case 0xc:
    _draw_sprite(param_1,_game_data[0x5c8],param_2 << 5,param_3 << 5);
    break;
  case 0xd:
    _draw_sprite(param_1,_game_data[0x5cc],param_2 << 5,param_3 << 5);
    break;
  case 0xe:
    _draw_sprite_h_flip(param_1,_game_data[0x5b0],param_2 << 5,param_3 << 5);
    break;
  case 0xf:
    _draw_sprite_h_flip(param_1,_game_data[0x5b4],param_2 << 5,param_3 << 5);
    break;
  case 0x10:
    _draw_sprite_h_flip(param_1,_game_data[0x5b8],param_2 << 5,param_3 << 5);
    break;
  case 0x11:
    _draw_sprite_h_flip(param_1,_game_data[0x5bc],param_2 << 5,param_3 << 5);
  }
  for (local_8 = 0; local_8 < 4; local_8 = local_8 + 1) {
    if ((param_2 == (char)((&_start)[local_8 * 4] * '\x02') >> 1) &&
       (param_3 == (short)(*(short *)(&_start + local_8 * 4) << 2) >> 9)) {
      if (((byte)(&DAT_00079c85)[local_8 * 4] >> 6 & 1) == 0) {
        _draw_sprite(param_1,_game_data[0x5d0],param_2 << 5,param_3 << 5);
      }
      else {
        _draw_sprite_h_flip(param_1,_game_data[0x5d0],param_2 << 5,param_3 << 5);
      }
    }
  }
  return;
}

