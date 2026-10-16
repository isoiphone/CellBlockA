/* JAILED.EXE address: 00002ba4 */

void _draw_tile(BITMAP *dest,int x,int y)

{
  int local_8;
  
  _blit(_game_data->dat,dest,x << 5,y << 5,x << 5,y << 5,0x20,0x20);
  switch(_map[x][y].kind) {
  case '\x04':
    _draw_sprite(dest,_game_data[0x1be].dat,x << 5,y << 5);
    break;
  case '\x06':
    _draw_sprite(dest,_game_data[0x1bf].dat,x << 5,y << 5);
    break;
  case '\a':
    _draw_sprite(dest,_game_data[0x1c0].dat,x << 5,y << 5);
    break;
  case '\b':
    _draw_sprite(dest,_game_data[0x1c1].dat,x << 5,y << 5);
    break;
  case '\t':
    _draw_sprite(dest,_game_data[0x1c2].dat,x << 5,y << 5);
    break;
  case '\n':
    _draw_sprite(dest,_game_data[0x1c3].dat,x << 5,y << 5);
    break;
  case '\v':
    _draw_sprite(dest,_game_data[0x1c4].dat,x << 5,y << 5);
    break;
  case '\f':
    _draw_sprite(dest,_game_data[0x1c5].dat,x << 5,y << 5);
    break;
  case '\r':
    _draw_sprite(dest,_game_data[0x1c6].dat,x << 5,y << 5);
    break;
  case '\x0e':
    _draw_sprite_h_flip(dest,_game_data[0x1bf].dat,x << 5,y << 5);
    break;
  case '\x0f':
    _draw_sprite_h_flip(dest,_game_data[0x1c0].dat,x << 5,y << 5);
    break;
  case '\x10':
    _draw_sprite_h_flip(dest,_game_data[0x1c1].dat,x << 5,y << 5);
    break;
  case '\x11':
    _draw_sprite_h_flip(dest,_game_data[0x1c2].dat,x << 5,y << 5);
  }
  for (local_8 = 0; local_8 < 4; local_8 = local_8 + 1) {
    if ((x == (char)((char)_start[local_8].packed * '\x02') >> 1) &&
       (y == (short)(_start[local_8].packed << 2) >> 9)) {
      if ((*(byte *)((int)&_start[local_8].packed + 1) >> 6 & 1) == 0) {
        _draw_sprite(dest,_game_data[0x1c7].dat,x << 5,y << 5);
      }
      else {
        _draw_sprite_h_flip(dest,_game_data[0x1c7].dat,x << 5,y << 5);
      }
    }
  }
  return;
}

