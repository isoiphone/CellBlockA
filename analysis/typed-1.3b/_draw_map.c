/* Original 1.3b CELL.EXE address: 0000b478 */

void _draw_map(void)

{
  undefined4 local_c;
  undefined4 local_8;
  
  for (local_8 = 0; local_8 < 0x14; local_8 = local_8 + 1) {
    for (local_c = 0; local_c < 0xd; local_c = local_c + 1) {
      _draw_tile(_buffer,local_8,local_c);
    }
  }
  _blit(native_arena_background,_buffer,0,0x19f,0,0x19f,0x27f,0x41);
  return;
}

