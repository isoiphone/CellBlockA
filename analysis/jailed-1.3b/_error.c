/* JAILED.EXE address: 00001574 */

void _error(int code,char *message)

{
  _destroy_bitmap(_buffer);
  _destroy_bitmap(DAT_00079ca0);
  _unload_datafile(_game_data);
  _allegro_exit();
  if (code == 0) {
    _fprintf((FILE *)&___dj_stderr,"%s\n",message);
  }
  else {
    _fprintf((FILE *)&___dj_stderr,"ERROR #%i: %s\n",code,message);
  }
                    /* WARNING: Subroutine does not return */
  _exit(code);
}

