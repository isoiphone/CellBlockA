/* Original 1.3b CELL.EXE address: 00006b10 */

void _attack_rebar(int param_1)

{
  _player[param_1].attack_wait = 0x19;
  _play_sound(0x1bb,_player[param_1].x);
  return;
}

