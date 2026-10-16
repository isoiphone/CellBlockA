/* Original CELL.EXE address: 00013670 */

int _fixtoi(int x)

{
  return ((int)(x & 0x8000U) >> 0xf) + (x >> 0x10);
}

