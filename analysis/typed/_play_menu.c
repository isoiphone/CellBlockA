/* Original CELL.EXE address: 0000511c */

undefined4 _play_menu(void)

{
  char *pcVar1;
  size_t sVar2;
  int iVar3;
  int iVar4;
  undefined4 local_16c;
  char local_168 [80];
  char local_118 [256];
  int local_18;
  int local_14;
  int local_10;
  FileNode *local_c;
  BITMAP *local_8;
  
  local_14 = 0;
  local_18 = 0;
  _get_map_filenames();
  _fade_out(3);
  if (_gfx_driver == (GFX_DRIVER *)0x0) {
    iVar3 = 0;
    iVar4 = 0;
  }
  else {
    iVar3 = _gfx_driver->h;
    iVar4 = _gfx_driver->w;
  }
  _blit(_menu_data[5].dat,_screen,0,0,0,0,iVar4,iVar3);
  _fade_in(_game_data[10].dat,3);
  _set_pallete(_game_data[10].dat);
  while( true ) {
    if (_gfx_driver == (GFX_DRIVER *)0x0) {
      iVar3 = 0;
      iVar4 = 0;
    }
    else {
      iVar3 = _gfx_driver->h;
      iVar4 = _gfx_driver->w;
    }
    _blit(_menu_data[5].dat,_buffer,0,0,0,0,iVar4,iVar3);
    local_c = _maphead.next;
    for (local_10 = 0; local_10 < local_14; local_10 = local_10 + 1) {
      local_c = local_c->next;
    }
    for (local_10 = 0; (local_10 < 0x24 && (local_10 + local_14 < _num_maps));
        local_10 = local_10 + 1) {
      if (local_18 == local_14 + local_10) {
        local_16c = 0x6f;
        _strcpy(local_118,"maps/");
        pcVar1 = _get_filename(local_c->name);
        _strcat(local_118,pcVar1);
        _strcpy(local_168,"maps/");
        pcVar1 = _get_filename(local_c->name);
        _strcat(local_168,pcVar1);
        sVar2 = _strlen(local_168);
        local_168[sVar2 - 3] = 'p';
        sVar2 = _strlen(local_168);
        local_168[sVar2 - 2] = 'i';
        sVar2 = _strlen(local_168);
        local_168[sVar2 - 1] = 'c';
        iVar3 = _exists(local_168);
        if (iVar3 != 0) {
          local_8 = _load_pcx(local_168,(RGB *)&__current_pallete);
          _blit(local_8,_buffer,0,0,0x1a6,0x20,200,0x96);
          _destroy_bitmap(local_8);
        }
      }
      else {
        local_16c = 0x65;
      }
      pcVar1 = _get_filename(local_c->name);
      _strcpy(local_168,pcVar1);
      _strupr(local_168);
      _textout(_buffer,(FONT *)_font,local_168,0x1e,local_10 * 10 + 0x32,local_16c);
      local_c = local_c->next;
    }
    if (_key[1] != '\0') break;
    if ((_key[0x1c] == '\0') || (iVar3 = _exists(local_118), iVar3 == 0)) {
      if (_key[0x48] == '\0') {
        if (_key[0x50] != '\0') {
          iVar3 = local_18 + 1;
          if ((iVar3 < _num_maps) && (local_18 = iVar3, local_14 + 0x24 <= iVar3)) {
            local_14 = local_14 + 1;
          }
          _key[0x50] = '\0';
        }
      }
      else {
        local_18 = local_18 + -1;
        if (local_18 < 0) {
          local_18 = 0;
        }
        else if (local_18 < local_14) {
          local_14 = local_14 + -1;
        }
        _key[0x48] = '\0';
      }
      if (_gfx_driver == (GFX_DRIVER *)0x0) {
        iVar3 = 0;
        iVar4 = 0;
      }
      else {
        iVar3 = _gfx_driver->h;
        iVar4 = _gfx_driver->w;
      }
      _blit(_buffer,_screen,0,0,0,0,iVar4,iVar3);
    }
    else {
      _clear_keybuf();
      _play_game(local_118);
      if (_gfx_driver == (GFX_DRIVER *)0x0) {
        iVar3 = 0;
        iVar4 = 0;
      }
      else {
        iVar3 = _gfx_driver->h;
        iVar4 = _gfx_driver->w;
      }
      _blit(_menu_data[5].dat,_screen,0,0,0,0,iVar4,iVar3);
      _clear_keybuf();
    }
  }
  _set_pallete(_game_data[10].dat);
  _fade_out(3);
  return 0;
}

