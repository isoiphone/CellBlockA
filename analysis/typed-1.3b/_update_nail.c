/* Original 1.3b CELL.EXE address: 0000554c */

void _update_nail(void)

{
  if (_selector.active == 0) {
    if (_selector.tick + 8 < 0x11) {
      _selector.tick = _selector.tick + 1;
    }
    else {
      _selector.tick = 0;
    }
  }
  return;
}

