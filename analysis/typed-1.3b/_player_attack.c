/* Original 1.3b CELL.EXE address: 00007360 */

void _player_attack(int param_1)

{
  int iVar1;
  
  if ((('\0' < _player[param_1].health) &&
      (((_player[param_1].state == 2 || (_player[param_1].state == 0)) &&
       (_player[param_1].attack_wait < 1)))) && (_player[param_1].stun == 0)) {
    _player[param_1].state = 6;
    iVar1 = _player[param_1].weapon;
    if (iVar1 == 1) {
      _player[param_1].frame = 0xaf;
      _attack_rebar(param_1);
    }
    else if (iVar1 == 0) {
      _player[param_1].frame = 0x13b;
      _attack_fists(param_1);
    }
    else if (iVar1 == 2) {
      if (_player[param_1].ammo < 1) {
        _player[param_1].weapon = 0;
      }
      else {
        _player[param_1].frame = 0xf0;
        _attack_shotgun(param_1);
      }
    }
    else if (iVar1 == 3) {
      _attack_fireext(param_1);
    }
  }
  return;
}

