/* Original CELL.EXE address: 00013764 */

int _fcos(int x)

{
  uint uVar1;
  
  if ((x & 0x4000U) == 0) {
    uVar1 = x >> 0xd & 0x7fc;
  }
  else {
    uVar1 = ((x >> 0xf) + 1U & 0x1ff) << 2;
  }
  return *(int *)((int)&__cos_tbl + uVar1);
}

