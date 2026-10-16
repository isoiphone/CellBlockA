/* Original CELL.EXE address: 000513e8 */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _rand(void)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = (uint)((ulonglong)_data * 0x4c957f2d);
  uVar2 = uVar1 + 1;
  DAT_0007117c = (int)((ulonglong)_data * 0x4c957f2d >> 0x20) + _data * 0x5851f42d +
                 DAT_0007117c * 0x4c957f2d + (uint)(0xfffffffe < uVar1);
  _data = uVar2;
  return uVar2 >> 0x15 | DAT_0007117c * 0x800 & 0x7fffffffU;
}

