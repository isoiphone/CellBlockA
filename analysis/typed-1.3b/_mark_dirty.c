/* Original 1.3b CELL.EXE address: 00001998 */

void _mark_dirty(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined1 local_c;
  undefined1 local_8;
  
  iVar1 = param_1;
  if (param_1 < 0) {
    iVar1 = param_1 + 0x1f;
  }
  iVar2 = param_2;
  if (param_2 < 0) {
    iVar2 = param_2 + 0x1f;
  }
  local_c = (char)(iVar2 >> 5);
  local_8 = (char)(iVar1 >> 5);
  _dirty((int)local_8,(int)local_c);
  iVar1 = param_1;
  if (param_1 < 0) {
    iVar1 = param_1 + 0x1f;
  }
  iVar2 = param_2;
  if (param_2 < 0) {
    iVar2 = param_2 + 0x1f;
  }
  local_c = (char)(iVar2 >> 5) + '\x01';
  local_8 = (char)(iVar1 >> 5);
  _dirty((int)local_8,(int)local_c);
  iVar1 = param_1;
  if (param_1 < 0) {
    iVar1 = param_1 + 0x1f;
  }
  local_8 = (char)(iVar1 >> 5) + '\x01';
  iVar1 = param_2;
  if (param_2 < 0) {
    iVar1 = param_2 + 0x1f;
  }
  local_c = (char)(iVar1 >> 5);
  _dirty((int)local_8,(int)local_c);
  if (param_1 < 0) {
    param_1 = param_1 + 0x1f;
  }
  local_8 = (char)(param_1 >> 5) + '\x01';
  if (param_2 < 0) {
    param_2 = param_2 + 0x1f;
  }
  local_c = (char)(param_2 >> 5) + '\x01';
  _dirty((int)local_8,(int)local_c);
  return;
}

