/* Original CELL.EXE address: 00004b94 */

int _main(int argc,char **argv)

{
  int iVar1;
  uint seed;
  int local_8;
  
  for (local_8 = 0; local_8 < argc; local_8 = local_8 + 1) {
    iVar1 = _strcmp(argv[local_8],"BLOODBATH!");
    if (iVar1 == 0) {
      _BLOODBATH = 1;
    }
    iVar1 = _strcmp(argv[local_8],"-nosound");
    if (iVar1 == 0) {
      _nosound = 1;
    }
  }
  _allegro_init();
  _install_keyboard();
  _fade_out(3);
  iVar1 = _set_gfx_mode(0,0x280,0x1e0,0,0);
  if (iVar1 < 0) {
    _error(1,"setting 640x480x256!");
  }
  _clear(_screen);
  seed = _rawclock();
  _srand(seed);
  _install_timer();
  _reserve_voices(0x20,-1);
  if (_nosound == 0) {
    iVar1 = _install_sound(-1,0,(char *)0x0);
    if (iVar1 < 0) {
      _error(2,"initialising sound");
    }
  }
  if (_nosound == 0) {
    iVar1 = _install_mod(0x10);
    if (iVar1 < 0) {
      _error(3,"setting digi voices");
    }
  }
  _game_data = _load_datafile("game.dat");
  if (_game_data == (DATAFILE *)0x0) {
    _error(3,"loading game.dat!");
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
  if ((_BLOODBATH != 0) && (_nosound == 0)) {
    _play_sample(_game_data[0x156].dat,0xff,0x80,1000,-1);
  }
  while (_playing != 0) {
    _main_menu();
  }
  if ((_BLOODBATH != 0) && (_nosound == 0)) {
    _stop_sample(_game_data[0x156].dat);
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
  _printf("Cell Block A - Version %s\n",&LC35);
  return 0;
}

