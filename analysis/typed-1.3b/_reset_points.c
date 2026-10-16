/* Original 1.3b CELL.EXE address: 00001ea4 */

void _reset_points(void)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  Point *pPVar4;
  int local_24 [7];
  int local_8;
  
  piVar2 = native_initial_point;
  piVar3 = local_24;
  for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
    *piVar3 = *piVar2;
    piVar2 = piVar2 + 1;
    piVar3 = piVar3 + 1;
  }
  for (local_8 = 0; local_8 < 0x100; local_8 = local_8 + 1) {
    piVar2 = local_24;
    pPVar4 = _point + local_8;
    for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
      pPVar4->life = *piVar2;
      piVar2 = piVar2 + 1;
      pPVar4 = (Point *)&pPVar4->x;
    }
  }
  return;
}

