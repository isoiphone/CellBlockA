/* JAILED.EXE address: 0000283c */

undefined4 _mappic(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 local_c;
  undefined4 local_8;
  
  uVar1 = _create_bitmap(0x280,0x1e0);
  uVar2 = _create_bitmap(200,0x96,uVar1);
  for (local_8 = 0; local_8 < 0x14; local_8 = local_8 + 1) {
    for (local_c = 0; local_c < 0xd; local_c = local_c + 1) {
      _draw_tile(uVar1,local_8,local_c);
    }
  }
  _blit(*_game_data,uVar1,0,0x19f,0,0x19f,0x27f,0x41);
  _stretch_blit(uVar1,uVar2,0,0,0x280,0x1e0,0,0,200,0x96);
  _destroy_bitmap(uVar1);
  return uVar2;
}

