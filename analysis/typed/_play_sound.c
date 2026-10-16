/* Original CELL.EXE address: 00001880 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _play_sound(int sample,int x)

{
  if (_nosound == 0) {
    _play_sample(_game_data[sample].dat,(int)ROUND(_LC10 * (double)_game_config.sound),
                 (int)ROUND(_LC9 * (double)x - 1.0),1000,0);
  }
  return;
}

