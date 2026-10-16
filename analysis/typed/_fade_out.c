/* Original CELL.EXE address: 00018d84 */

void _fade_out(int speed)

{
  undefined1 local_400 [1024];
  
  _get_pallete_range(local_400,0,0xff);
  _fade_from_range(local_400,&_black_pallete,speed,0,0xff);
  return;
}

