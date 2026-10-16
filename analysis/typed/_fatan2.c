/* Original CELL.EXE address: 0001e6fc */

int _fatan2(int y,int x)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  bool bVar8;
  
  if (x == 0) {
    if (y == 0) {
      _errno = 1;
      iVar1 = 0;
    }
    else {
      iVar1 = 0x400000;
      if (y < 0) {
        iVar1 = -0x400000;
      }
    }
  }
  else {
    _errno = 0;
    bVar8 = y < 0;
    uVar2 = y;
    if (bVar8) {
      uVar2 = -y;
    }
    uVar4 = x;
    if (x < 0) {
      uVar4 = -x;
      bVar8 = !bVar8;
    }
    if ((uVar4 <= uVar2 >> 0x10) ||
       (iVar3 = (int)(CONCAT44(uVar2 >> 0x10,uVar2 << 0x10) / (ulonglong)uVar4), iVar3 < 0)) {
      _errno = 2;
      iVar3 = 0x7fffffff;
    }
    if (bVar8) {
      iVar3 = -iVar3;
    }
    if (iVar3 < 0) {
      iVar7 = 0x80;
      iVar5 = 0xff;
    }
    else {
      iVar7 = 0;
      iVar5 = 0x7f;
    }
    do {
      iVar1 = iVar5 + iVar7 >> 1;
      iVar6 = iVar3 - *(int *)(&__tan_tbl + iVar1 * 4);
      if (iVar6 < 1) {
        if (iVar6 < 0) {
          iVar5 = iVar1 + -1;
        }
      }
      else {
        iVar7 = iVar1 + 1;
      }
    } while ((iVar7 <= iVar5) && (iVar3 != *(int *)(&__tan_tbl + iVar1 * 4)));
    if (iVar3 < 0) {
      iVar1 = iVar1 * 0x8000 + -0x800000;
    }
    else {
      iVar1 = iVar1 << 0xf;
    }
    if (x < 0) {
      if (y < 0) {
        iVar1 = iVar1 + -0x800000;
      }
      else {
        iVar1 = iVar1 + 0x800000;
      }
    }
  }
  return iVar1;
}

