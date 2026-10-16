/* Original 1.3b CELL.EXE address: 0000658c */

int _find_angle(int x1,int y1,int x2,int y2)

{
  int x;
  int y;
  uint uVar1;
  
  x = _itofix(x2 - x1);
  y = _itofix(y2 - y1);
  uVar1 = _fatan2(y,x);
  return uVar1 & 0xff0000;
}

