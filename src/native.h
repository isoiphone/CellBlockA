#ifndef CELL_NATIVE_H
#define CELL_NATIVE_H
/* Keep the recovered name in C, but avoid the ELF executable entry symbol. */
#define _start cell_player_starts
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef uint8_t byte, uchar, undefined1;
typedef uint16_t ushort, undefined2;
typedef uint32_t uint, undefined4;
typedef unsigned char undefined;
#define ROUND(x) (x) /* Original x87 control word selects truncation on these casts. */
#undef NAN
#define NAN(x) isnan(x)
#define CONCAT11(a,b) ((uint16_t)(((uint16_t)(uint8_t)(a)<<8)|(uint8_t)(b)))
#ifdef __cplusplus
extern "C" {
#endif
typedef struct Player {
 int fallen, x, y, height, field10, field14, vx, vy;
 char facing; char pad21[3];
 int frame, state;
 signed char health; char pad2d[3];
 int attack_wait, roll_wait, stun, weapon, ammo, up, down, left, right, field54;
} Player;
typedef struct Point { int life, x, y, vx, vy; unsigned char color; char pad[3]; int type; } Point;
typedef struct Tile { unsigned char kind, dirty; } Tile;
typedef struct Start { unsigned short packed; unsigned short unused; } Start;
typedef struct Selector { int frame, y, tick, active; } Selector;
typedef struct Config { int sound, music, maxkills, maxtime; } Config;
typedef struct ModNode {char name[80];struct ModNode *prev,*next;} ModNode;
typedef struct FileNode { char name[80]; struct FileNode *next; } FileNode;
typedef struct BITMAP { int w,h; unsigned char *pixels; } BITMAP;
typedef struct FONT { int height; BITMAP *glyph[224]; } FONT;
typedef struct SAMPLE { int bits, stereo, freq, priority; unsigned int len, loop_start, loop_end; int param; void *data; } SAMPLE;
typedef struct JGMOD MOD;
typedef struct RGB { unsigned char r,g,b,filler; } RGB;
typedef struct DATAFILE { void *dat; int type, size; void *prop; } DATAFILE;
typedef struct GFX_DRIVER { char pad[28]; int w,h; } GFX_DRIVER;
typedef struct PACKFILE { unsigned char *data; unsigned long size,pos; void *file; } PACKFILE;
int _hit(int x,int y);
int _collide(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
void _control_player(Player *p,Player *other);
void _adjust_player(Player *p,Player *other,int index);
void _translate_player(int index);
void _damage_player(int index,char damage,int x,int y,int reason);
void _spawn_particles(int x,int y,int count,int color,int type);
void _play_sound(int sample,int x);
int _distance(int x1,int y1,int x2,int y2);
int _find_angle(int x1,int y1,int x2,int y2);
void _draw_tile(BITMAP *dest,int x,int y);
void _load_map(char *filename);
int _play_game(char *filename);
void _error(int code,char *message);
void _spawn_shotgun(void);
void _spawn_rebar(void);
void _spawn_fireext(void);
void _spawn_tp(void);
void _get_map_filenames(void);
void _free_map_filenames(void);
void _get_mod_filenames(void);
void _free_mod_filenames(void);
char *_getmatchlength(void);
void _move_player_vertical(void);
void _move_player_horisontal(void);
int _itofix(int x);
int _fixtoi(int x);
int _ftofix(double x);
int _fsin(int x);
int _fcos(int x);
int _fatan2(int y,int x);
double _sqrt(double x);
void _blit(BITMAP *src,BITMAP *dst,int sx,int sy,int dx,int dy,int w,int h);
void _draw_sprite(BITMAP *dst,BITMAP *src,int x,int y);
void _draw_sprite_h_flip(BITMAP *dst,BITMAP *src,int x,int y);
void _clear(BITMAP *dst);
void _rectfill(BITMAP *dst,int x1,int y1,int x2,int y2,int c);
void _putpixel(BITMAP *dst,int x,int y,int c);
void _textout(BITMAP *dst,FONT *font,char *text,int x,int y,int c);
void _textprintf(BITMAP *dst,FONT *font,int x,int y,int c,char *fmt,...);
void _textprintf_centre(BITMAP *dst,FONT *font,int x,int y,int c,char *fmt,...);
BITMAP *_create_bitmap(int w,int h);
void _destroy_bitmap(BITMAP *bmp);
BITMAP *_load_pcx(char *path,RGB *pal);
int _save_pcx(char *path,BITMAP *bmp,RGB *pal);
DATAFILE *_load_datafile(char *path);
void _unload_datafile(DATAFILE *data);
void _set_pallete(RGB *pal);
void _fade_in(RGB *pal,int speed);
void _fade_out(int speed);
int _set_gfx_mode(int mode,int w,int h,int vw,int vh);
int _allegro_init(void);
void _allegro_exit(void);
int _install_keyboard(void);
int _install_timer(void);
int _install_sound(int digi,int midi,char *cfg);
void _reserve_voices(int digi,int midi);
int _install_mod(int voices);
void _set_mod_volume(int volume);
MOD *_load_mod(char *path);
void _destroy_mod(MOD *mod);
void _play_mod(MOD *mod,int loop);
void _stop_mod(void);
int _is_mod_playing(void);
int _is_mod_paused(void);
void _pause_mod(void);
void _resume_mod(void);
int _play_sample(SAMPLE *s,int volume,int pan,int freq,int loop);
void _stop_sample(SAMPLE *s);
int _install_int(void (*func)(void),int ms);
int _install_int_ex(void (*func)(void),int ticks);
void _remove_int(void (*func)(void));
int __go32_dpmi_lock_data(void *p,int length);
void _vsync(void);
int _keypressed(void);
int _readkey(void);
void _clear_keybuf(void);
int _clock(void);
unsigned int _rawclock(void);
int _rand(void);
void _srand(unsigned int seed);
void _set_config_file(char *path);
int _get_config_int(char *section,char *key,int fallback);
void _set_config_int(char *section,char *key,int value);
PACKFILE *_pack_fopen(char *path,char *mode);
int _pack_getc(PACKFILE *f);
int _pack_fclose(PACKFILE *f);
int _exists(char *path);
char *_get_filename(char *path);
char *cell_strupr(char *s);

int _main(int argc,char **argv);

int _printf(char *format,...);

/* Map accesses are bounded; no dependency on adjacent player/file-list data. */
extern Tile _map[20][13];
extern Player _player[2];
extern Point _point[256];
extern Start _start[4];
extern Selector _selector;
extern Config _game_config;
extern FileNode _maphead;
extern ModNode _modhead, *_cursong;
extern DATAFILE *_game_data, *_menu_data;
extern BITMAP *_buffer, *_screen, *native_arena_background;
extern GFX_DRIVER *_gfx_driver;
extern MOD *_music;
extern FONT *_font;
extern RGB _black_pallete[256], __current_pallete[256];
extern char *_cur_map;
extern double _game_time;
extern int _match_start, _endmatch, _barrel_x, _barrel_y;
extern int _playing, _BLOODBATH, _nosound, _num_mods, _num_maps, _lastspot;
extern int _going, _player1_wins, _player2_wins, _match, _frame_counter, _last_fps, _show_fps;
unsigned char *native_keys(void);
#define _key (native_keys())
void native_pump(void);
void native_present(void);
void native_shutdown(void);
uint64_t native_counter_micros(uint64_t counter,uint64_t frequency);
#define _strcmp strcmp
#define _strcpy strcpy
#define _strcat strcat
#define _strlen strlen
#define _sprintf sprintf
#define _printf printf
#define _fprintf fprintf
#define _malloc malloc
#define _free free
#define _exit exit
#define ___dj_stderr stderr
void _adjust_player(Player *p,Player *other,int index);
void _attack_fireext(int param_1);
void _attack_fists(int param_1);
void _attack_rebar(int param_1);
void _attack_shotgun(int param_1);
void _clean_map(void);
int _collide(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
void _control_player(Player *p,Player *other);
void _crunchsound(int param_1);
void _damage_player(int index,char damage,int x,int y,int reason);
void _dirty(char param_1,char param_2);
int _distance(int x1,int y1,int x2,int y2);
void _draw_map(void);
void _draw_nail(void);
void _draw_player(int param_1);
void _draw_points(void);
void _draw_tile(BITMAP *dest,int x,int y);
void _end_match(void);
void _error(int code,char *message);
int _find_angle(int x1,int y1,int x2,int y2);
void _fire_bullet(int param_1,float param_2,float param_3,float param_4);
void _fps_timer(void);
void _fps_timer_end(void);
void _game_tick(void);
void _get_keys(void);
char * _getmatchlength(void);
undefined4 _help_menu(void);
int _hit(int x,int y);
undefined4 _hit_player(int param_1);
void _item_timer(void);
void _load_map(char *filename);
int _main(int argc,char **argv);
undefined4 _main_menu(void);
void _make_dirty(int x1,int y1,int x2,int y2);
void _mark_dirty(int param_1,int param_2);
void _move_player_horisontal(void);
void _move_player_vertical(void);
void _next_song(void);
void _options_menu(void);
int _play_game(char *filename);
undefined4 _play_menu(void);
void _play_sound(int sample,int x);
void _player_attack(int param_1);
void _prev_song(void);
void _rand_song(void);
void _reset_map(void);
void _reset_points(void);
void _spawn_fireext(void);
void _spawn_particles(int x,int y,int count,int color,int type);
void _spawn_player(int param_1);
void _spawn_rebar(void);
void _spawn_shotgun(void);
void _spawn_stuff(void);
void _spawn_tp(void);
void _start_match(void);
void _start_music(void);
char * _state(int param_1);
void _translate_player(int index);
void _translate_points(void);
void _update_nail(void);

#ifdef __cplusplus
}
#endif
#endif
