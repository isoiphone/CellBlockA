/* Original CELL.EXE address: 00004084 */

void _draw_points(void)

{
  int iVar1;
  int y;
  int iVar2;
  int local_14;
  int local_8;
  
  for (local_8 = 0; local_8 < 0x100; local_8 = local_8 + 1) {
    if (0 < _point[local_8].life) {
      local_14 = _fixtoi(_point[local_8].x);
      if (local_14 < 0) {
        local_14 = local_14 + 0x1f;
      }
      if (local_14 >> 5 < 0x14) {
        local_14 = _fixtoi(_point[local_8].y);
        if (local_14 < 0) {
          local_14 = local_14 + 0x1f;
        }
        if (local_14 >> 5 < 0xd) {
          iVar1 = _fixtoi(_point[local_8].x);
          local_14 = _fixtoi(_point[local_8].y);
          if (local_14 < 0) {
            local_14 = local_14 + 0x1f;
          }
          if (iVar1 < 0) {
            iVar1 = iVar1 + 0x1f;
          }
          _map[iVar1 >> 5][local_14 >> 5].dirty = '\x01';
        }
      }
      iVar1 = _fixtoi(_point[local_8].x);
      y = _fixtoi(_point[local_8].y);
      switch(_point[local_8].type) {
      case 0:
        _putpixel(_buffer,iVar1,y,(uint)_point[local_8].color);
        break;
      case 1:
        iVar2 = _ftofix(0.1);
        if (iVar2 < _point[local_8].vy) {
          if (_point[local_8].vx < 1) {
            _draw_sprite_h_flip(_buffer,_game_data[_point[local_8].life % 6 + 0x175].dat,iVar1 + -10
                                ,y + -8);
          }
          else {
            _draw_sprite(_buffer,_game_data[_point[local_8].life % 6 + 0x175].dat,iVar1 + -10,y + -8
                        );
          }
        }
        else {
          _draw_sprite(_buffer,_game_data[0x17c].dat,iVar1 + -10,y + -8);
        }
        _mark_dirty(iVar1 + -10,y + -8);
        break;
      case 2:
        _draw_sprite(_buffer,_game_data[0xd].dat,iVar1,y);
        _mark_dirty(iVar1,y);
        break;
      case 3:
        _draw_sprite(_buffer,_game_data[0xc].dat,iVar1,y);
        _mark_dirty(iVar1,y);
        break;
      case 4:
        _draw_sprite(_buffer,_game_data[0xb].dat,iVar1,y + -10);
        _mark_dirty(iVar1,y + -10);
      }
    }
  }
  return;
}

