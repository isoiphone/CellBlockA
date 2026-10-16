/* Original 1.3b CELL.EXE address: 00007780 */

void _damage_player(int index,char damage,int x,int y,int reason)

{
  int iVar1;
  char local_c;
  char local_5;
  
  local_5 = damage;
  if (_player[index].health < '\x01') {
    return;
  }
  if (((_BLOODBATH == 0) || (reason == 0)) || (reason == 5)) {
    _spawn_particles(x,y,damage + 10,0,0);
  }
  else {
    iVar1 = _rand();
    local_c = (char)iVar1 + (char)(iVar1 / 5) * -5;
    local_5 = local_c + '\x01';
    _spawn_particles(x,y,local_5 + 0x20,0,0);
  }
  _player[index].stun = 0;
  _player[index].health = _player[index].health - local_5;
  if ('\0' < _player[index].health) {
    if (index == 0) {
      iVar1 = _rand();
      _play_sound(iVar1 % 0x1e + 0x15c,x);
    }
    else {
      iVar1 = _rand();
      _play_sound(iVar1 % 0x16 + 0x193,x);
    }
    goto LAB_00007f73;
  }
  _endmatch = 0x4b;
  if (_player[index].height != 0x19) {
    _player[index].height = 0x19;
    iVar1 = _player[index].y;
    if (iVar1 < 0) {
      iVar1 = iVar1 + 0x1f;
    }
    _player[index].y = ((iVar1 >> 5) * 0x20 + 0x20) - _player[index].height;
  }
  if (reason == 2) {
    if (index == 0) {
      _player[0].facing = _player[1].facing == '\0';
    }
    else if (_player[0].facing == '\0') {
      _player[index].facing = '\x01';
    }
    else {
      _player[index].facing = '\0';
    }
    _player[index].state = 7;
    _player[index].frame = 0x2c;
  }
  else if (reason < 3) {
    if (reason == 0) {
      _player[index].state = 9;
      _player[index].frame = 0x10e;
      _crunchsound(x);
    }
    else {
LAB_00007dfc:
      _player[index].state = 7;
      _player[index].frame = 0x2c;
    }
  }
  else if (reason == 3) {
    _play_sound(0x1b3,x);
    iVar1 = _distance(_player[0].x + 8,_player[0].y,_player[1].x + 8,_player[1].y);
    if (iVar1 < 0x1e) {
      _player[index].state = 8;
      _player[index].frame = 0xe;
      _play_sound(0x1bc,x);
    }
    else {
      _player[index].state = 7;
      _player[index].frame = 0x2c;
    }
  }
  else {
    if (reason != 5) goto LAB_00007dfc;
    iVar1 = _rand();
    _play_sound(0x1bd,iVar1 % 0x280);
    _player[index].state = 8;
    _player[index].frame = 0xe;
    _play_sound(0x1bc,x);
  }
  if (index == 0) {
    iVar1 = _rand();
    _play_sound(iVar1 % 0x1e + 0x13e,x);
  }
  else {
    iVar1 = _rand();
    _play_sound(iVar1 % 0x19 + 0x17a,x);
  }
LAB_00007f73:
  if (_player[index].health < '\0') {
    _player[index].health = '\0';
  }
  return;
}

