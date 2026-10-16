/* Original 1.3b CELL.EXE address: 00005438 */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _free_map_filenames(void)

{
  FileNode *pFVar1;
  FileNode *local_c;
  
  local_c = _maphead.next;
  while (local_c != (FileNode *)0x0) {
    pFVar1 = local_c->next;
    _free(local_c);
    local_c = pFVar1;
  }
  _num_maps = 0;
  return;
}

