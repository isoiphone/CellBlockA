/* Original CELL.EXE address: 0000abc8 */

void _draw_tile(BITMAP *dest,int x,int y)

{
  _blit(_game_data->dat,dest,x << 5,y << 5,x << 5,y << 5,0x20,0x20);
  switch(_map[x][y].kind) {
  case '\x04':
    _draw_sprite(dest,_game_data[0x16b].dat,x << 5,y << 5);
    break;
  case '\x06':
    _draw_sprite(dest,_game_data[0x16c].dat,x << 5,y << 5);
    break;
  case '\a':
    _draw_sprite(dest,_game_data[0x16d].dat,x << 5,y << 5);
    break;
  case '\b':
    _draw_sprite(dest,_game_data[0x16e].dat,x << 5,y << 5);
    break;
  case '\t':
    _draw_sprite(dest,_game_data[0x16f].dat,x << 5,y << 5);
    break;
  case '\n':
    _draw_sprite(dest,_game_data[0x170].dat,x << 5,y << 5);
    break;
  case '\v':
    _draw_sprite(dest,_game_data[0x171].dat,x << 5,y << 5);
    break;
  case '\f':
    _draw_sprite(dest,_game_data[0x172].dat,x << 5,y << 5);
    break;
  case '\r':
    _draw_sprite(dest,_game_data[0x173].dat,x << 5,y << 5);
    break;
  case '\x0e':
    _draw_sprite_h_flip(dest,_game_data[0x16c].dat,x << 5,y << 5);
    break;
  case '\x0f':
    _draw_sprite_h_flip(dest,_game_data[0x16d].dat,x << 5,y << 5);
    break;
  case '\x10':
    _draw_sprite_h_flip(dest,_game_data[0x16e].dat,x << 5,y << 5);
    break;
  case '\x11':
    _draw_sprite_h_flip(dest,_game_data[0x16f].dat,x << 5,y << 5);
  }
  _map[x][y].dirty = '\0';
  return;
}

