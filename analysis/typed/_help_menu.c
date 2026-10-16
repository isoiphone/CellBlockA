/* Original CELL.EXE address: 000055c0 */

undefined4 _help_menu(void)

{
  int iVar1;
  int iVar2;
  
  _clear_keybuf();
  _fade_out(3);
  if (_gfx_driver == (GFX_DRIVER *)0x0) {
    iVar1 = 0;
    iVar2 = 0;
  }
  else {
    iVar1 = _gfx_driver->h;
    iVar2 = _gfx_driver->w;
  }
  _blit(_menu_data->dat,_screen,0,0,0,0,iVar2,iVar1);
  _fade_in(_menu_data[0x11].dat,3);
  _readkey();
  _fade_out(3);
  _clear_keybuf();
  if (_gfx_driver == (GFX_DRIVER *)0x0) {
    iVar1 = 0;
    iVar2 = 0;
  }
  else {
    iVar1 = _gfx_driver->h;
    iVar2 = _gfx_driver->w;
  }
  _blit(_menu_data[1].dat,_screen,0,0,0,0,iVar2,iVar1);
  _fade_in(_menu_data[0x12].dat,3);
  _readkey();
  _fade_out(3);
  _clear_keybuf();
  if (_gfx_driver == (GFX_DRIVER *)0x0) {
    iVar1 = 0;
    iVar2 = 0;
  }
  else {
    iVar1 = _gfx_driver->h;
    iVar2 = _gfx_driver->w;
  }
  _blit(_menu_data[2].dat,_screen,0,0,0,0,iVar2,iVar1);
  _fade_in(_menu_data[0x13].dat,3);
  _readkey();
  _fade_out(3);
  return 0;
}

