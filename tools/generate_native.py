#!/usr/bin/env python3
"""Compile-ready transcription of typed decompiler evidence; no x86 interpreter."""
from pathlib import Path
import re, json, struct
from extract_original import address_bytes
root=Path(__file__).resolve().parents[1]
src=root/'src'; src.mkdir(exist_ok=True)
h=(root/'analysis/types-1.3b.h').read_text()
h=h.replace('_strupr', 'cell_strupr') # Avoid the Windows CRT symbol of the same name.
h=h.replace('signed char damage','char damage')
h=h.replace('typedef struct BITMAP { int w,h; } BITMAP;', 'typedef struct BITMAP { int w,h; unsigned char *pixels; } BITMAP;')
h=h.replace('typedef struct FONT { int height; } FONT;', 'typedef struct FONT { int height; BITMAP *glyph[224]; } FONT;')
h=h.replace('typedef struct SAMPLE { int bits; } SAMPLE;', 'typedef struct SAMPLE { int bits, stereo, freq, priority; unsigned int len, loop_start, loop_end; int param; void *data; } SAMPLE;')
h=h.replace('typedef struct MOD { char name[32]; } MOD;', 'typedef struct JGMOD MOD;')
h=h.replace('typedef struct PACKFILE { int dummy; } PACKFILE;', 'typedef struct PACKFILE { unsigned char *data; unsigned long size,pos; void *file; } PACKFILE;')
h='''#ifndef CELL_NATIVE_H
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
'''+h+'''
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
'''
metadata=json.loads((root/'analysis/recovery-1.3b.json').read_text())
exclude={'_get_map_filenames','_free_map_filenames','_get_mod_filenames','_free_mod_filenames'}
texts=[]
for p in sorted((root/'analysis/typed-1.3b').glob('*.c')):
 if p.stem in exclude or p.stem in {'_rand','_srand','_ftofix','_fixtoi','_itofix','_fcos','_fsin','_fatan2','_fade_in','_fade_out','_fade_from_range'}: continue
 s=p.read_text()
 s=s.replace('_strupr','cell_strupr')
 # Preserve integer width; widen ONLY the compiler's integer casts of native pointers.
 s=s.replace('(int)&','(uintptr_t)&').replace('(Player *)0x816cc','(_player + 1)')
 s=s.replace('local_2c._0_1_', '(*(signed char *)&local_2c)')
 if p.stem=='_hit':
  s=s.replace('{','{\n  /* Approved safety fix: outside the map is empty space. */\n  if (x < 0 || x >= 640 || y < 0 || y >= 416) return 0;\n',1)
 if p.stem=='_make_dirty':
  s='''/* Original 1.3b CELL.EXE address: 0000c410 */
/* Approved safety fix: clip the inclusive pixel rectangle before indexing. */
void _make_dirty(int x1,int y1,int x2,int y2)

{
  if (x1 > x2 || y1 > y2 || x2 < 0 || y2 < 0 || x1 >= 640 || y1 >= 416)
    return;
  if (x1 < 0) x1 = 0;
  if (y1 < 0) y1 = 0;
  if (x2 >= 640) x2 = 639;
  if (y2 >= 416) y2 = 415;
  for (int x = x1 / 32; x <= x2 / 32; ++x)
    for (int y = y1 / 32; y <= y2 / 32; ++y)
      _map[x][y].dirty = 1;
}
'''
 s=s.replace('*(FileNode **)_cursong[1].name','_cursong->next')
 if p.stem=='_prev_song': s=s.replace('_cursong->next','_cursong->prev')
 if p.stem=='_getmatchlength':
  s=s.replace('char *local_20;','static char match_time[32];\n  char *local_20 = match_time;')
 # Read the original initialization block below; its final type word is zero.
 s=s.replace('native_initial_point','initial_point')
 s=s.replace('&DAT_00004c48',json.dumps(metadata['version']))
 # Direct DOS stdout/stderr handles are platform concerns.
 s=s.replace('&___dj_stderr','stderr')
 m=re.search(r'\n([\w *]+ _\w+\([^;]*?\))\n\n\{',s)
 if m: h+=m.group(1)+';\n'
 texts.append(s)
# Bit-identical constants read from their original addresses.
sy=metadata['symbols']; b=(root/'analysis/CELL.coff').read_bytes()
if address_bytes(b,metadata['version_string_address'],5) != b'1.3b\0':
 raise RuntimeError('Source generation requires the verified 1.3b executable')
# Tables and type catalog are reproducible from the original, not regenerated
# using host floating point or guessed asset labels.
tables=[]
for symbol,name,count in [('__cos_tbl','cos_table',512),('__tan_tbl','tan_table',256)]:
 a=sy[symbol]
 values=struct.unpack(f'<{count}i',address_bytes(b,a,count*4))
 tables.append(f'static const int {name}[{count}]={{'+','.join(map(str,values))+'};')
(src/'trig_tables.h').write_text('/* Exact Allegro 3.0 lookup tables from CELL.EXE. */\n'+'\n'.join(tables)+'\n')
catalog=[]
for group in ['GAME','MENU']:
 entries=json.loads((root/'analysis/assets'/group/'index.json').read_text())
 catalog.append(f'static const char *{group.lower()}_types[]={{'+','.join('"'+x['type']+'"' for x in entries)+'};')
(src/'asset_types.h').write_text('\n'.join(catalog)+'\n')
constants=[]
for n, item in metadata['constants'].items():
 a=item['address']; isfloat=item['type']=='float'
 v=struct.unpack('<f' if isfloat else '<d',address_bytes(b,a,4 if isfloat else 8))[0]
 constants.append(f'static const {item["type"]} {n} = {v.hex()}{"f" if isfloat else ""}; /* 1.3b {a:#x} */')
h+='''
#ifdef __cplusplus
}
#endif
#endif
'''
(src/'native.h').write_text(h)
initial=struct.unpack('<7i',address_bytes(b,metadata['initial_point_address'],28))
(src/'recovered.c').write_text('/* Generated by tools/generate_native.py from analysis/typed-1.3b (CELL.EXE 1.3b). */\n#include "native.h"\nstatic int initial_point[7]={'+','.join(map(str,initial))+'};\n'+'\n'.join(constants)+'\n\n'+'\n'.join(texts))
