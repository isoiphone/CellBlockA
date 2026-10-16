/* JAILED.EXE address: 00001a78 */

undefined4 _hit(int param_1,int param_2)

{
  byte bVar1;
  
  if (param_2 < 0) {
    param_2 = param_2 + 0x1f;
  }
  if (param_1 < 0) {
    param_1 = param_1 + 0x1f;
  }
  bVar1 = (&_map)[(param_2 >> 5) * 2 + (param_1 >> 5) * 0x1a];
  if (bVar1 != 0) {
    if (bVar1 == 4) {
      return 2;
    }
    if ((5 < bVar1) && (bVar1 < 0x12)) {
      return 1;
    }
  }
  return 0;
}

