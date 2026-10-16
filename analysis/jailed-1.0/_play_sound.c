/* JAILED.EXE address: 00001880 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _play_sound(int param_1,int param_2)

{
  if (_nosound == 0) {
    _play_sample(*(undefined4 *)(_game_data + param_1 * 0x10),
                 (int)ROUND(_LC10 * (double)__game_config),(int)ROUND(_LC9 * (double)param_2 - 1.0),
                 1000,0);
  }
  return;
}

