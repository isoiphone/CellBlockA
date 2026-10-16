/* Original 1.3b CELL.EXE address: 00005490 */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _get_map_filenames(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 local_130 [30];
  char local_112 [262];
  FileNode *local_c;
  FileNode *local_8;
  
  uVar2 = 0;
  _free_map_filenames();
  local_c = &_maphead;
  iVar1 = _findfirst("maps/*.cel",local_130,1,uVar2);
  while (iVar1 == 0) {
    local_8 = _malloc(0x54);
    _strcpy(local_8->name,"maps/");
    _strcat(local_8->name,local_112);
    local_c->next = local_8;
    local_8->next = (FileNode *)0x0;
    local_c = local_8;
    _num_maps = _num_maps + 1;
    iVar1 = _findnext(local_130);
  }
  return;
}

