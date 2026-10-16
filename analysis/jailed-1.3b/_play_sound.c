/* JAILED.EXE address: 0000188c */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _play_sound(int sample,int x)

{
  if (_nosound == 0) {
    _play_sample(_game_data[sample].dat,(int)ROUND(_DAT_00001884 * (double)__game_config),
                 (int)ROUND(_DAT_0000187c * (double)x - 1.0),1000,0);
  }
  return;
}

