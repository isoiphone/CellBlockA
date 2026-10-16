/* JAILED.EXE address: 0000192c */

void _dirty(char param_1,char param_2)

{
  if ((((param_1 < '\x14') && (-1 < param_1)) && (param_2 < '\r')) && (-1 < param_2)) {
    _map[param_1][param_2].dirty = '\x01';
  }
  return;
}

