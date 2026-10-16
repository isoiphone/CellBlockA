/* Original CELL.EXE address: 00004930 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _item_timer(void)

{
  double dVar1;
  int iVar2;
  int local_10;
  int local_c;
  
  local_10 = 0;
  for (local_c = 0; local_c < 0x100; local_c = local_c + 1) {
    if ((_point[local_c].life != 0) && (_point[local_c].type != 0)) {
      local_10 = local_10 + 1;
    }
  }
  if (local_10 < 3) {
    iVar2 = _clock();
    dVar1 = (double)(int)ROUND(_LC17 * (double)(iVar2 - _match_start));
    iVar2 = _rand();
    iVar2 = iVar2 % 0xb;
    if ((iVar2 < 7) || (8 < iVar2)) {
      if ((iVar2 < 4) || ((6 < iVar2 || (dVar1 <= _LC18)))) {
        if ((iVar2 < 1) || ((3 < iVar2 || (dVar1 <= _LC19)))) {
          if (_LC20 < dVar1) {
            _spawn_shotgun();
          }
        }
        else {
          _spawn_fireext();
        }
      }
      else {
        _spawn_rebar();
      }
    }
    else {
      _spawn_tp();
    }
  }
  return;
}

