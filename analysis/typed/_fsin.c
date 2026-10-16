/* Original CELL.EXE address: 0001378c */

int _fsin(int x)

{
  uint uVar1;
  
  if ((x & 0x4000U) == 0) {
    uVar1 = (x >> 0xf) - 0x80;
  }
  else {
    uVar1 = (x >> 0xf) - 0x7f;
  }
  return (int)(&__cos_tbl)[uVar1 & 0x1ff];
}

