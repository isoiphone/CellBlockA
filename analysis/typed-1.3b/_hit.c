/* Original 1.3b CELL.EXE address: 00001a84 */

int _hit(int x,int y)

{
  byte bVar1;
  
  if (y < 0) {
    y = y + 0x1f;
  }
  if (x < 0) {
    x = x + 0x1f;
  }
  bVar1 = _map[x >> 5][y >> 5].kind;
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

