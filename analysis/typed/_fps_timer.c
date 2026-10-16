/* Original CELL.EXE address: 0000aba4 */

void _fps_timer(void)

{
  _last_fps = _frame_counter;
  _frame_counter = 0;
  return;
}

