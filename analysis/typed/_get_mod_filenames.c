/* Original CELL.EXE address: 00001b88 */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _get_mod_filenames(void)

{
  int iVar1;
  undefined1 local_134 [30];
  char local_116 [262];
  FileNode *local_10;
  FileNode *local_c;
  FileNode *local_8;
  
  _free_mod_filenames();
  iVar1 = _findfirst("music/*.mod",local_134,1);
  if (iVar1 == 0) {
    local_8 = &_modhead;
    local_10 = &_modhead;
    do {
      local_c = _malloc(0x58);
      _strcpy(local_c->name,"music/");
      _strcat(local_c->name,local_116);
      local_8->next = local_10;
      *(FileNode **)local_8[1].name = local_c;
      local_c->next = local_8;
      *(FileNode **)local_c[1].name = &_modhead;
      _modhead.next = local_c;
      local_8 = local_c;
      local_10 = local_c->next;
      _num_mods = _num_mods + 1;
      iVar1 = _findnext(local_134);
    } while (iVar1 == 0);
  }
  return;
}

