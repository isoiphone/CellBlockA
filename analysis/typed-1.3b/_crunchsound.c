/* Original 1.3b CELL.EXE address: 0000c2f0 */

void _crunchsound(int param_1)

{
  int iVar1;
  
  iVar1 = _rand();
  _play_sound(iVar1 % 3 + 0x1aa,param_1);
  return;
}

