/* Original CELL.EXE address: 00005064 */

void _update_nail(void)

{
  if (_selector.active == 0) {
    if (_selector.tick + 6 < 0xf) {
      _selector.tick = _selector.tick + 1;
    }
    else {
      _selector.tick = 0;
    }
  }
  return;
}

