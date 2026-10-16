/* Original CELL.EXE address: 0000afdc */

void _clean_map(void)

{
  int local_c;
  int local_8;
  
  for (local_8 = 0; local_8 < 0x14; local_8 = local_8 + 1) {
    for (local_c = 0; local_c < 0xd; local_c = local_c + 1) {
      if (_map[local_8][local_c].dirty != '\0') {
        _draw_tile(_buffer,local_8,local_c);
      }
    }
  }
  if ((0x16d < _player[0].y) || (0x16d < _player[1].y)) {
    _blit(_game_data->dat,_buffer,0,0x19f,0,0x19f,0x27f,0x41);
  }
  return;
}

