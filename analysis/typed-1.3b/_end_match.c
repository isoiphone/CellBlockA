/* Original 1.3b CELL.EXE address: 00008288 */

void _end_match(void)

{
  char *pcVar1;
  int iVar2;
  
  _remove_int(_item_timer);
  _remove_int(_fps_timer);
  _remove_int(_game_tick);
  _remove_int(_translate_points);
  if ((_player[1].health == '\0') && (_player[0].health != '\0')) {
    _player1_wins = _player1_wins + 1;
    _draw_sprite(_screen,_game_data[0x1d1].dat,0x80,0x76);
    _textprintf(_screen,_game_data[6].dat,0x108,0xcd,0xf4,"%2d",_match);
    pcVar1 = _getmatchlength();
    _textprintf(_screen,_game_data[6].dat,0x16d,0xeb,0xf4,"%s",pcVar1);
    _textprintf(_screen,_game_data[6].dat,0x13e,0x127,0xf4,"%2d",_player1_wins);
    _textprintf(_screen,_game_data[6].dat,0x157,0x145,0xf4,"%2d",_player2_wins);
  }
  else if ((_player[0].health == '\0') && (_player[1].health != '\0')) {
    _player2_wins = _player2_wins + 1;
    _draw_sprite(_screen,_game_data[0x1d2].dat,0x80,0x76);
    _textprintf(_screen,_game_data[6].dat,0x109,0xcd,0xf4,"%2d",_match);
    pcVar1 = _getmatchlength();
    _textprintf(_screen,_game_data[6].dat,0x16e,0xeb,0xf4,"%s",pcVar1);
    _textprintf(_screen,_game_data[6].dat,0x13f,0x127,0xf4,"%2d",_player2_wins);
    _textprintf(_screen,_game_data[6].dat,0x158,0x145,0xf4,"%2d",_player1_wins);
  }
  else {
    _draw_sprite(_screen,_game_data[0x1d0].dat,0x80,0x76);
    _textprintf(_screen,_game_data[6].dat,0x110,0xd1,0xf4,"%2d",_match);
    pcVar1 = _getmatchlength();
    _textprintf(_screen,_game_data[6].dat,0x171,0xef,0xf4,"%s",pcVar1);
    _textprintf(_screen,_game_data[6].dat,0x19a,299,0xf4,"%2d",_player1_wins);
    _textprintf(_screen,_game_data[6].dat,0x19a,0x149,0xf4,"%2d",_player2_wins);
  }
  _clear_keybuf();
  do {
    iVar2 = _keypressed();
  } while (iVar2 == 0);
  if (0 < _game_config.maxkills) {
    if (_game_config.maxkills < _player1_wins) {
      _draw_sprite(_screen,_game_data[0x1d3].dat,0x80,0x76);
      _player1_wins = 0;
      _player2_wins = 0;
      do {
      } while (_key[0x39] == '\0');
    }
    else if (_game_config.maxkills < _player2_wins) {
      _draw_sprite(_screen,_game_data[0x1d4].dat,0x80,0x76);
      _player1_wins = 0;
      _player2_wins = 0;
      do {
      } while (_key[0x39] == '\0');
    }
  }
  return;
}

