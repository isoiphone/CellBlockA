/* Original 1.3b CELL.EXE address: 0000b0bc */

void _fps_timer(void)

{
  _last_fps = _frame_counter;
  _frame_counter = 0;
  return;
}

