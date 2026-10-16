/* Original CELL.EXE address: 00017e34 */

void _fade_from_range(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  byte *pbVar4;
  byte *pbVar5;
  byte *local_410;
  int local_40c;
  int local_404;
  byte local_400 [1024];
  
  local_404 = 0;
  iVar3 = 0;
  do {
    *(undefined4 *)(local_400 + iVar3) = *(undefined4 *)(param_1 + iVar3);
    iVar3 = iVar3 + 4;
    local_404 = local_404 + 1;
  } while (local_404 < 0x100);
  local_404 = 0;
  do {
    local_40c = param_4;
    if (param_4 <= param_5) {
      iVar2 = 0x3f - local_404;
      iVar3 = param_4 * 4;
      local_410 = local_400 + iVar3;
      pbVar5 = (byte *)(param_2 + iVar3);
      pbVar4 = (byte *)(param_1 + iVar3);
      do {
        iVar3 = (uint)*pbVar5 * local_404 + (uint)*pbVar4 * iVar2;
        if (iVar3 < 0) {
          iVar3 = iVar3 + 0x3f;
        }
        *local_410 = (byte)(iVar3 >> 6);
        iVar3 = (uint)*(byte *)(param_2 + 1 + local_40c * 4) * local_404 +
                (uint)*(byte *)(param_1 + 1 + local_40c * 4) * iVar2;
        if (iVar3 < 0) {
          iVar3 = iVar3 + 0x3f;
        }
        local_400[local_40c * 4 + 1] = (byte)(iVar3 >> 6);
        iVar3 = (uint)*(byte *)(param_2 + 2 + local_40c * 4) * local_404 +
                (uint)*(byte *)(param_1 + 2 + local_40c * 4) * iVar2;
        if (iVar3 < 0) {
          iVar3 = iVar3 + 0x3f;
        }
        local_400[local_40c * 4 + 2] = (byte)(iVar3 >> 6);
        local_410 = local_410 + 4;
        pbVar5 = pbVar5 + 4;
        pbVar4 = pbVar4 + 4;
        local_40c = local_40c + 1;
      } while (local_40c <= param_5);
    }
    __check_gfx_virginity();
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      __vga_set_pallete_range(local_400,param_4,param_5,0xffffffff);
    }
    else if (*(int *)(_screen[3].h + 4) == 8) {
      (**(code **)(_gfx_driver->pad + 0x18))(local_400,param_4,param_5,0xffffffff);
    }
    if (param_4 <= param_5) {
      pbVar5 = local_400 + param_4 * 4;
      iVar3 = param_4;
      do {
        (&__current_pallete)[iVar3] = *(undefined4 *)pbVar5;
        if (__color_depth != 8) {
          uVar1 = _makecol(*(undefined4 *)(&__rgb_scale_6 + (uint)*pbVar5 * 4),
                           *(undefined4 *)(&__rgb_scale_6 + (uint)local_400[iVar3 * 4 + 1] * 4),
                           *(undefined4 *)(&__rgb_scale_6 + (uint)local_400[iVar3 * 4 + 2] * 4));
          (&_pallete_color)[iVar3] = uVar1;
        }
        pbVar5 = pbVar5 + 4;
        iVar3 = iVar3 + 1;
      } while (iVar3 <= param_5);
    }
    __check_gfx_virginity();
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      __vga_set_pallete_range(local_400,param_4,param_5,0xffffffff);
    }
    else if (*(int *)(_screen[3].h + 4) == 8) {
      (**(code **)(_gfx_driver->pad + 0x18))(local_400,param_4,param_5,0xffffffff);
    }
    if (param_4 <= param_5) {
      pbVar5 = local_400 + param_4 * 4;
      iVar3 = param_4;
      do {
        (&__current_pallete)[iVar3] = *(undefined4 *)pbVar5;
        if (__color_depth != 8) {
          uVar1 = _makecol(*(undefined4 *)(&__rgb_scale_6 + (uint)*pbVar5 * 4),
                           *(undefined4 *)(&__rgb_scale_6 + (uint)local_400[iVar3 * 4 + 1] * 4),
                           *(undefined4 *)(&__rgb_scale_6 + (uint)local_400[iVar3 * 4 + 2] * 4));
          (&_pallete_color)[iVar3] = uVar1;
        }
        pbVar5 = pbVar5 + 4;
        iVar3 = iVar3 + 1;
      } while (iVar3 <= param_5);
    }
    local_404 = local_404 + param_3;
  } while (local_404 < 0x40);
  __check_gfx_virginity();
  if (_gfx_driver == (GFX_DRIVER *)0x0) {
    __vga_set_pallete_range(param_2,param_4,param_5,0xffffffff);
  }
  else if (*(int *)(_screen[3].h + 4) == 8) {
    (**(code **)(_gfx_driver->pad + 0x18))(param_2,param_4,param_5,0xffffffff);
  }
  if (param_4 <= param_5) {
    pbVar5 = (byte *)(param_2 + param_4 * 4);
    do {
      (&__current_pallete)[param_4] = *(undefined4 *)pbVar5;
      if (__color_depth != 8) {
        uVar1 = _makecol(*(undefined4 *)(&__rgb_scale_6 + (uint)*pbVar5 * 4),
                         *(undefined4 *)
                          (&__rgb_scale_6 + (uint)*(byte *)(param_2 + 1 + param_4 * 4) * 4),
                         *(undefined4 *)
                          (&__rgb_scale_6 + (uint)*(byte *)(param_2 + 2 + param_4 * 4) * 4));
        (&_pallete_color)[param_4] = uVar1;
      }
      pbVar5 = pbVar5 + 4;
      param_4 = param_4 + 1;
    } while (param_4 <= param_5);
  }
  return;
}

