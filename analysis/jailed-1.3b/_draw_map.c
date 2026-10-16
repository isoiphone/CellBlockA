/* JAILED.EXE address: 00002288 */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _draw_map(void)

{
  int local_c;
  int local_8;
  
  for (local_8 = 0; local_8 < 0x14; local_8 = local_8 + 1) {
    for (local_c = 0; local_c < 0xd; local_c = local_c + 1) {
      _draw_tile(_buffer,local_8,local_c);
    }
  }
  _blit(_game_data->dat,_buffer,0,0x19f,0,0x19f,0x27f,0x41);
  return;
}

