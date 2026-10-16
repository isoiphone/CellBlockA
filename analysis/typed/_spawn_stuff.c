/* Original CELL.EXE address: 000048b0 */

void _spawn_stuff(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = _rand();
  iVar2 = iVar1;
  if (iVar1 < 0) {
    iVar2 = iVar1 + 3;
  }
  iVar1 = iVar1 + (iVar2 >> 2) * -4;
  if (iVar1 == 1) {
    _spawn_shotgun();
  }
  else if (iVar1 < 2) {
    if (iVar1 == 0) {
      _spawn_tp();
    }
  }
  else if (iVar1 == 2) {
    _spawn_rebar();
  }
  else if (iVar1 == 3) {
    _spawn_fireext();
  }
  return;
}

