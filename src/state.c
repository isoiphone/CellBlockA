#include "native.h"
Tile _map[20][13];
Player _player[2];
Point _point[256];
Start _start[4];
Selector _selector;
Config _game_config;
FileNode _maphead;
ModNode _modhead, *_cursong;
DATAFILE *_game_data, *_menu_data;
BITMAP *_buffer, *_screen, *native_arena_background;
GFX_DRIVER *_gfx_driver;
MOD *_music;
FONT *_font;
RGB _black_pallete[256], __current_pallete[256];
char *_cur_map;
double _game_time;
int _match_start, _endmatch, _barrel_x, _barrel_y;
/* Initial values read directly from original 1.3b CELL.EXE's .data section. */
int _playing=1, _BLOODBATH=0, _nosound=0, _num_mods=0, _num_maps=0, _lastspot=-1;
int _going=1, _player1_wins=0, _player2_wins=0, _match=0, _frame_counter=0, _last_fps=0, _show_fps=0;
