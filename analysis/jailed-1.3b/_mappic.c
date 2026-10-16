/* JAILED.EXE address: 00002848 */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

BITMAP * _mappic(void)

{
  BITMAP *dest;
  BITMAP *dst;
  int local_c;
  int local_8;
  
  dest = _create_bitmap(0x280,0x1e0);
  dst = _create_bitmap(200,0x96);
  for (local_8 = 0; local_8 < 0x14; local_8 = local_8 + 1) {
    for (local_c = 0; local_c < 0xd; local_c = local_c + 1) {
      _draw_tile(dest,local_8,local_c);
    }
  }
  _blit(_game_data->dat,dest,0,0x19f,0,0x19f,0x27f,0x41);
  _stretch_blit(dest,dst,0,0,0x280,0x1e0,0,0,200,0x96);
  _destroy_bitmap(dest);
  return dst;
}

