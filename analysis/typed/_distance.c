/* Original CELL.EXE address: 00006078 */

int _distance(int x1,int y1,int x2,int y2)

{
  double dVar1;
  
  dVar1 = _sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
  return (int)ROUND(dVar1);
}

