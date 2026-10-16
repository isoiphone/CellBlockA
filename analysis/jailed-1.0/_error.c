/* JAILED.EXE address: 00001574 */

void _error(int param_1,undefined4 param_2)

{
  _destroy_bitmap(_buffer);
  _unload_datafile(_game_data);
  _allegro_exit();
  if (param_1 == 0) {
    _fprintf((FILE *)&___dj_stderr,"%s\n",param_2);
  }
  else {
    _fprintf((FILE *)&___dj_stderr,"ERROR #%i: %s\n",param_1,param_2);
  }
  _exit(param_1);
  return;
}

