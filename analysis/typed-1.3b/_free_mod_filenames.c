/* Original 1.3b CELL.EXE address: 00001b0c */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _free_mod_filenames(void)

{
  ModNode *pMVar1;
  ModNode *local_8;
  
  local_8 = (ModNode *)_modhead.next;
  while (local_8 != (ModNode *)0x0) {
    pMVar1 = (ModNode *)local_8->next;
    _free(local_8);
    local_8 = pMVar1;
  }
  _strcpy(_modhead.name,"");
  _modhead.next = (ModNode **)&_modhead;
  _modhead.prev = &_modhead;
  _cursong = &_modhead;
  _num_mods = 0;
  return;
}

