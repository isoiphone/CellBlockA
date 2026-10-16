/* Original 1.3b CELL.EXE address: 0000aa98 */

void _draw_player(int param_1)

{
  uint uVar1;
  
  uVar1 = _player[param_1].state;
  if (uVar1 == 4) {
    _draw_sprite(_buffer,_game_data[_player[param_1].frame].dat,_player[param_1].x + -0x1e,
                 _player[param_1].y + -0x14);
    _make_dirty(_player[param_1].x + -0x1e,_player[param_1].y + -0x14,_player[param_1].x + 0x2f,
                _player[param_1].y + 0x26);
    goto LAB_0000affc;
  }
  if (uVar1 < 5) {
    if (uVar1 == 3) {
      if (_player[param_1].facing == '\0') {
        _draw_sprite_h_flip(_buffer,_game_data[_player[param_1].frame].dat,
                            _player[param_1].x + -0x18,_player[param_1].y + -0x21);
      }
      else {
        _draw_sprite(_buffer,_game_data[_player[param_1].frame].dat,_player[param_1].x + -0x18,
                     _player[param_1].y + -0x21);
      }
      _make_dirty(_player[param_1].x + -0x18,_player[param_1].y + -0x21,_player[param_1].x + 0x28,
                  _player[param_1].y + 0xf);
      goto LAB_0000affc;
    }
  }
  else if (uVar1 == 6) {
    if (_player[param_1].weapon == 1) {
      if (_player[param_1].facing == '\0') {
        _draw_sprite_h_flip(_buffer,_game_data[_player[param_1].frame].dat,
                            _player[param_1].x + -0x1f,_player[param_1].y + -0x1c);
      }
      else {
        _draw_sprite(_buffer,_game_data[_player[param_1].frame].dat,_player[param_1].x + -0x1f,
                     _player[param_1].y + -0x1c);
      }
      _make_dirty(_player[param_1].x + -0x18,_player[param_1].y + -0x14,_player[param_1].x + 0x28,
                  _player[param_1].y + 0x17);
      goto LAB_0000affc;
    }
    if (_player[param_1].weapon == 3) {
      if ((_player[param_1].frame < 0x122) || (0x13a < _player[param_1].frame)) {
        if (_player[param_1].facing == '\0') {
          _draw_sprite_h_flip(_buffer,_game_data[_player[param_1].frame].dat,
                              _player[param_1].x + -0x18,_player[param_1].y + -0x18);
        }
        else {
          _draw_sprite(_buffer,_game_data[_player[param_1].frame].dat,_player[param_1].x + -0x18,
                       _player[param_1].y + -0x18);
        }
        _make_dirty(_player[param_1].x + -0x18,_player[param_1].y + -0x14,_player[param_1].x + 0x28,
                    _player[param_1].y + 0x17);
      }
      else if (_player[param_1].facing == '\0') {
        _draw_sprite_h_flip(_buffer,_game_data[_player[param_1].frame].dat,
                            _player[param_1].x + -0x30,_player[param_1].y + -0x18);
        _make_dirty(_player[param_1].x + -0x30,_player[param_1].y + -0x14,_player[param_1].x + 0x10,
                    _player[param_1].y + 0x17);
      }
      else {
        _draw_sprite(_buffer,_game_data[_player[param_1].frame].dat,_player[param_1].x,
                     _player[param_1].y + -0x18);
        _make_dirty(_player[param_1].x,_player[param_1].y + -0x14,_player[param_1].x + 0x40,
                    _player[param_1].y + 0x17);
      }
      goto LAB_0000affc;
    }
  }
  else if (uVar1 == 9) {
    if (_player[param_1].facing == '\0') {
      _draw_sprite_h_flip(_buffer,_game_data[_player[param_1].frame].dat,_player[param_1].x + -0x18,
                          _player[param_1].y);
    }
    else {
      _draw_sprite(_buffer,_game_data[_player[param_1].frame].dat,_player[param_1].x + -0x18,
                   _player[param_1].y);
    }
    _make_dirty(_player[param_1].x + -0x18,_player[param_1].y + -0x14,_player[param_1].x + 0x28,
                _player[param_1].y + 0x1c);
    goto LAB_0000affc;
  }
  if (_player[param_1].facing == '\0') {
    _draw_sprite_h_flip(_buffer,_game_data[_player[param_1].frame].dat,_player[param_1].x + -0x18,
                        _player[param_1].y + -0x14);
  }
  else {
    _draw_sprite(_buffer,_game_data[_player[param_1].frame].dat,_player[param_1].x + -0x18,
                 _player[param_1].y + -0x14);
  }
  _make_dirty(_player[param_1].x + -0x18,_player[param_1].y + -0x14,_player[param_1].x + 0x28,
              _player[param_1].y + 0x1c);
LAB_0000affc:
  _draw_sprite(_buffer,_game_data[param_1 + 0x80].dat,_player[param_1].x + 5,
               _player[param_1].y + -0x1e);
  _make_dirty(_player[param_1].x + 5,_player[param_1].y + -0x1e,_player[param_1].x + 10,
              _player[param_1].y + -0x16);
  return;
}

