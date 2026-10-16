/* Original CELL.EXE address: 00018d64 */

void _fade_in(RGB *pal,int speed)

{
  _fade_from_range(&_black_pallete,pal,speed,0,0xff);
  return;
}

