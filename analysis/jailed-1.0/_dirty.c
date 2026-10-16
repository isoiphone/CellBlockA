/* JAILED.EXE address: 00001920 */

void _dirty(char param_1,char param_2)

{
  if ((((param_1 < '\x14') && (-1 < param_1)) && (param_2 < '\r')) && (-1 < param_2)) {
    (&DAT_00079a69)[param_2 * 2 + param_1 * 0x1a] = 1;
  }
  return;
}

