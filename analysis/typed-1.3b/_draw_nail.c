/* Original 1.3b CELL.EXE address: 0000558c */

void _draw_nail(void)

{
  if (_selector.active == 0) {
    _draw_sprite(_buffer,_menu_data[_selector.tick + 8].dat,_selector.frame,_selector.y);
  }
  else if (_selector.active == 1) {
    _draw_sprite(_buffer,_menu_data[10].dat,_selector.frame,_selector.y);
  }
  return;
}

