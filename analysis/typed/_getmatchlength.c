/* Original CELL.EXE address: 0000bb84 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char * _getmatchlength(void)

{
  int iVar1;
  char *local_20;
  int local_18;
  
  iVar1 = _clock();
  local_18 = (int)ROUND(_LC68 * ((double)iVar1 - (double)_match_start));
  if (99 < local_18) {
    local_18 = 99;
  }
  _sprintf(local_20,"%2d:%2d",local_18,
           (int)ROUND(_LC69 * ((double)iVar1 - (double)_match_start)) % 0x3c);
  return local_20;
}

