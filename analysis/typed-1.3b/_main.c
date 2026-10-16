/* Original 1.3b CELL.EXE address: 00004c68 */

int _main(int argc,char **argv)

{
  bool bVar1;
  int iVar2;
  uint seed;
  int iVar3;
  int dy;
  BITMAP *dst;
  int sx;
  int sy;
  int w;
  int h;
  int local_14;
  int local_8;
  
  bVar1 = true;
  local_8 = 0;
  while( true ) {
    if (argc <= local_8) {
      _allegro_init();
      _install_keyboard();
      _fade_out(3);
      iVar2 = _set_gfx_mode(0,0x280,0x1e0,0,0);
      if (iVar2 < 0) {
        _error(1,"setting 640x480x256!");
      }
      _clear(_screen);
      seed = _rawclock();
      _srand(seed);
      _install_timer();
      _reserve_voices(0x20,-1);
      if ((_nosound == 0) && (iVar2 = _install_sound(-1,0,(char *)0x0), iVar2 < 0)) {
        _error(2,"initialising sound");
      }
      if ((_nosound == 0) && (iVar2 = _install_mod(0x10), iVar2 < 0)) {
        _error(3,"setting digi voices");
      }
      _game_data = _load_datafile("game.dat");
      if (_game_data == (DATAFILE *)0x0) {
        _error(3,"loading game.dat!");
      }
      _menu_data = _load_datafile("menu.dat");
      if (_menu_data == (DATAFILE *)0x0) {
        _error(3,"loading menu.dat!");
      }
      _buffer = _create_bitmap(0x280,0x1e0);
      _set_config_file("cell.cfg");
      _game_config.sound = _get_config_int("volume","sound",100);
      if (100 < _game_config.sound) {
        _game_config.sound = 100;
      }
      _game_config.music = _get_config_int("volume","music",0x28);
      if (100 < _game_config.music) {
        _game_config.music = 100;
      }
      _game_config.maxkills = _get_config_int("misc","maxkills",0);
      if (999 < _game_config.maxkills) {
        _game_config.maxkills = 999;
      }
      _game_config.maxtime = _get_config_int("misc","maxtime",0);
      if (999 < _game_config.maxtime) {
        _game_config.maxtime = 999;
      }
      _set_mod_volume(_game_config.music);
      _get_mod_filenames();
      _clear(_buffer);
      _set_pallete((RGB *)&_black_pallete);
      _font = _game_data[8].dat;
      if (bVar1) {
        _fade_out(3);
        if (_gfx_driver == (GFX_DRIVER *)0x0) {
          local_14 = 0;
          iVar2 = local_14;
          local_14 = 0;
        }
        else {
          iVar2 = _gfx_driver->h;
          local_14 = _gfx_driver->w;
        }
        _blit(_menu_data->dat,_screen,0,0,0,0,local_14,iVar2);
        _fade_in(_menu_data[0x16].dat,3);
        _set_pallete(_menu_data[0x16].dat);
        _readkey();
        _fade_out(3);
        _start_music();
        if (_gfx_driver == (GFX_DRIVER *)0x0) {
          local_14 = 0;
          iVar2 = local_14;
          local_14 = 0;
        }
        else {
          iVar2 = _gfx_driver->h;
          local_14 = _gfx_driver->w;
        }
        _blit(_menu_data[1].dat,_screen,0,0,0,0,local_14,iVar2);
        _fade_in(_menu_data[0x17].dat,3);
        _set_pallete(_menu_data[0x17].dat);
        _readkey();
      }
      else {
        _start_music();
      }
      if ((_BLOODBATH != 0) && (_nosound == 0)) {
        _play_sample(_game_data[0x1a9].dat,0xff,0x80,1000,-1);
      }
      native_arena_background = _create_bitmap(0x280,0x1e0);
      _blit(_game_data->dat,native_arena_background,0,0,0,0,0x280,0x1e0);
      h = 0x30;
      w = 0x40;
      iVar2 = _rand();
      dy = iVar2 % 0x50 + 400;
      iVar2 = _rand();
      iVar2 = iVar2 % 0x280;
      sy = 0;
      sx = 0;
      dst = native_arena_background;
      iVar3 = _rand();
      _blit(_game_data[iVar3 % 3 + 0x68].dat,dst,sx,sy,iVar2,dy,w,h);
      while (_playing != 0) {
        _main_menu();
      }
      _destroy_bitmap(native_arena_background);
      if ((_BLOODBATH != 0) && (_nosound == 0)) {
        _stop_sample(_game_data[0x1a9].dat);
      }
      _set_pallete((RGB *)&_black_pallete);
      _set_config_int("volume","sound",_game_config.sound);
      _set_config_int("volume","music",_game_config.music);
      _set_config_int("misc","maxkills",_game_config.maxkills);
      _set_config_int("misc","maxtime",_game_config.maxtime);
      _destroy_bitmap(_buffer);
      _destroy_mod(_music);
      _unload_datafile(_game_data);
      _allegro_exit();
      _printf("Cell Block A - Version %s\n",&DAT_00004c48);
      return 0;
    }
    iVar2 = _strcmp(argv[local_8],"-blade");
    if (iVar2 == 0) {
      _BLOODBATH = 1;
    }
    iVar2 = _strcmp(argv[local_8],"-nosound");
    if (iVar2 == 0) {
      _nosound = 1;
    }
    iVar2 = _strcmp(argv[local_8],"-nointro");
    if (iVar2 == 0) {
      bVar1 = false;
    }
    iVar2 = _strcmp(argv[local_8],"-credits");
    if (iVar2 == 0) break;
    local_8 = local_8 + 1;
  }
  _printf(
         "\nReal Name Credits\n------------------\nKevlarX = Jacob Schwartz\nCobol = Dustin Schwartz\nProtomas = Thomas Craft\nNecros = David Bronough\n\n- October 11, 1999. - \n"
         );
  return 0;
}

