/* Original 1.3b CELL.EXE address: 0000c410 */

void _make_dirty(int param_1,int param_2,int param_3,int param_4)

{
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  for (local_8 = param_1; local_8 < param_3; local_8 = local_8 + 0x10) {
    local_10 = local_8;
    if (local_8 < 0) {
      local_10 = local_8 + 0x1f;
    }
    local_10 = local_10 >> 5;
    if (0x13 < local_10) {
      local_10 = 0x13;
    }
    for (local_c = param_2; local_c < param_4; local_c = local_c + 0x10) {
      local_14 = local_c;
      if (local_c < 0) {
        local_14 = local_c + 0x1f;
      }
      local_14 = local_14 >> 5;
      if (0xc < local_14) {
        local_14 = 0xc;
      }
      _map[local_10][local_14].dirty = '\x01';
    }
  }
  local_10 = param_3;
  if (param_3 < 0) {
    local_10 = param_3 + 0x1f;
  }
  local_10 = local_10 >> 5;
  if (0x13 < local_10) {
    local_10 = 0x13;
  }
  if (param_4 < 0) {
    param_4 = param_4 + 0x1f;
  }
  local_14 = param_4 >> 5;
  if (0xc < local_14) {
    local_14 = 0xc;
  }
  _map[local_10][local_14].dirty = '\x01';
  if (param_3 < 0) {
    param_3 = param_3 + 0x1f;
  }
  local_10 = param_3 >> 5;
  if (0x13 < local_10) {
    local_10 = 0x13;
  }
  if (param_2 < 0) {
    param_2 = param_2 + 0x1f;
  }
  local_14 = param_2 >> 5;
  if (0xc < local_14) {
    local_14 = 0xc;
  }
  _map[local_10][local_14].dirty = '\x01';
  return;
}

