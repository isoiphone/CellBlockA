/* Original CELL.EXE address: 000047c0 */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _spawn_fireext(void)

{
  int local_10;
  
  local_10 = _rand();
  if (_gfx_driver == (GFX_DRIVER *)0x0) {
    local_10 = local_10 % 0;
  }
  else {
    local_10 = local_10 % _gfx_driver->w;
  }
  local_10 = local_10 + 1;
  _spawn_particles(local_10,1,1,0,4);
  return;
}

