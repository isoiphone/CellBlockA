/* Original CELL.EXE address: 000050a4 */

void _draw_nail(void)

{
  if (_selector.active == 0) {
    _draw_sprite(_buffer,_menu_data[_selector.tick + 6].dat,_selector.frame,_selector.y);
  }
  else if (_selector.active == 1) {
    _draw_sprite(_buffer,_menu_data[8].dat,_selector.frame,_selector.y);
  }
  return;
}

