/* Original 1.3b CELL.EXE address: 00001f0c */

void _spawn_particles(int x,int y,int count,int color,int type)

{
  int iVar1;
  int iVar2;
  int local_20;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_c = 0;
  local_10 = 0;
  switch(color) {
  case 0:
    local_10 = 0x60;
    local_c = 0x7f;
    break;
  case 1:
    local_10 = 0xa0;
    local_c = 0xaf;
    break;
  case 2:
    local_10 = 8;
    local_c = 0x15;
    break;
  case 3:
    local_10 = 0x18;
    local_c = 0x20;
    break;
  case 4:
    local_10 = 0x80;
    local_c = 0x9f;
  }
  local_8 = 0;
  do {
    if (0xff < local_8) {
      return;
    }
    if (_point[local_8].life < 1) {
      if (type == 1) {
        local_14 = _rand();
        local_14 = local_14 % 2;
      }
      else {
        local_14 = _rand();
        local_14 = local_14 % 3;
      }
      local_14 = local_14 + 1;
      iVar1 = _itofix(x);
      _point[local_8].x = iVar1;
      iVar1 = _itofix(y);
      _point[local_8].y = iVar1;
      iVar1 = _rand();
      local_20 = iVar1;
      if (iVar1 < 0) {
        local_20 = iVar1 + 0xff;
      }
      iVar1 = _itofix(iVar1 + (local_20 >> 8) * -0x100);
      iVar2 = _fcos(iVar1);
      _point[local_8].vx = iVar2 * local_14;
      iVar1 = _fsin(iVar1);
      _point[local_8].vy = iVar1 * local_14;
      if (color == 0) {
        iVar1 = _rand();
        _point[local_8].life = iVar1 % 0x23 + 0xf;
      }
      else if (color == 3) {
        iVar1 = _rand();
        _point[local_8].life = iVar1 % 5 + 5;
      }
      else {
        iVar1 = _rand();
        _point[local_8].life = iVar1 % 10 + 10;
      }
      iVar1 = _rand();
      _point[local_8].color = (char)local_10 + (char)(iVar1 % (local_c - local_10));
      _point[local_8].type = type;
      if (type == 2) {
        _point[local_8].life = 1000;
        _point[local_8].color = 0xff;
      }
      else if (type < 3) {
        if (type == 1) {
          _point[local_8].life = 1000;
          _point[local_8].color = '\x1f';
        }
      }
      else if (type == 3) {
        _point[local_8].life = 1000;
        _point[local_8].color = 0xff;
      }
      else if (type == 4) {
        _point[local_8].life = 1000;
        _point[local_8].color = 0xff;
      }
      count = count + -1;
      if (count == 0) {
        return;
      }
    }
    local_8 = local_8 + 1;
  } while( true );
}

