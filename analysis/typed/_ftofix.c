/* Original CELL.EXE address: 000136ac */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int _ftofix(double x)

{
  double dVar1;
  int iVar2;
  
  if (x <= _LC12) {
    if ((byte)((_LC13 < x | (byte)((ushort)((ushort)(NAN(_LC13) || NAN(x)) << 10) >> 8) |
               (byte)((ushort)((ushort)(_LC13 == x) << 0xe) >> 8)) - 1) < 0x40) {
      dVar1 = _LC16;
      if (0x3f < (byte)((0.0 < x | (byte)((ushort)((ushort)NAN(x) << 10) >> 8) |
                        (byte)((ushort)((ushort)(x == 0.0) << 0xe) >> 8)) - 1)) {
        dVar1 = _LC15;
      }
      iVar2 = (int)ROUND(dVar1 + x * _LC14);
    }
    else {
      _errno = 2;
      iVar2 = -0x80000000;
    }
  }
  else {
    _errno = 2;
    iVar2 = 0x7fffffff;
  }
  return iVar2;
}

