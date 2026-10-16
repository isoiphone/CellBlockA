/* Original CELL.EXE address: 00001b00 */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _free_mod_filenames(void)

{
  void *pvVar1;
  void *local_8;
  
  local_8 = DAT_00082bc0;
  while (local_8 != (void *)0x0) {
    pvVar1 = *(void **)((int)local_8 + 0x54);
    _free(local_8);
    local_8 = pvVar1;
  }
  _strcpy(_modhead.name,"");
  DAT_00082bc0 = &_modhead;
  _modhead.next = &_modhead;
  _cursong = &_modhead;
  _num_mods = 0;
  return;
}

