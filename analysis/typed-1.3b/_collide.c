/* Original 1.3b CELL.EXE address: 00007fd8 */

int _collide(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2)

{
  int iVar1;
  
  if ((((x2 + w2 < x1) || (w1 + x1 < x2)) || (y2 + h2 < y1)) || (y1 + h1 < y2)) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  return iVar1;
}

