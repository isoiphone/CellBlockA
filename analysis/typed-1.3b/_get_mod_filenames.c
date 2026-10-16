/* Original 1.3b CELL.EXE address: 00001b94 */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _get_mod_filenames(void)

{
  int iVar1;
  undefined1 local_134 [30];
  char local_116 [262];
  ModNode *local_10;
  ModNode *local_c;
  ModNode *local_8;
  
  _free_mod_filenames();
  iVar1 = _findfirst("music/*.mod",local_134,1);
  if (iVar1 == 0) {
    local_8 = &_modhead;
    local_10 = &_modhead;
    do {
      local_c = _malloc(0x58);
      _strcpy(local_c->name,"music/");
      _strcat(local_c->name,local_116);
      local_8->prev = local_10;
      local_8->next = (ModNode **)local_c;
      local_c->prev = local_8;
      local_c->next = (ModNode **)&_modhead;
      _modhead.prev = local_c;
      local_8 = local_c;
      local_10 = local_c->prev;
      _num_mods = _num_mods + 1;
      iVar1 = _findnext(local_134);
    } while (iVar1 == 0);
  }
  return;
}

