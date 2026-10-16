/* Original 1.3b CELL.EXE address: 000015f0 */

void _reset_map(void)

{
  int local_c;
  int local_8;
  
  for (local_8 = 0; local_8 < 0x14; local_8 = local_8 + 1) {
    for (local_c = 0; local_c < 0xd; local_c = local_c + 1) {
      _map[local_8][local_c].kind = '\0';
      _map[local_8][local_c].dirty = '\0';
    }
  }
  _start[0].packed = CONCAT11((char)(_start[0].packed >> 8),1);
  _start[0].packed = _start[0].packed & 0x807f | 0x100;
  _start[1].packed = CONCAT11((char)(_start[1].packed >> 8),0x12);
  _start[1].packed = _start[1].packed & 0xc07f;
  _start[1].packed = _start[1].packed | 0x4100;
  _start[2].packed = _start[2].packed & 0xbfff | 0x3fff;
  _start[3].packed = _start[3].packed & 0xbfff | 0x3fff;
  return;
}

