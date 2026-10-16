/* Original CELL.EXE address: 0005145c */

void _srand(uint seed)

{
  _data = seed;
  DAT_0007117c = 0;
  return;
}

