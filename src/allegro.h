/* Host API used by preserved JGMOD. No DOS or CPU emulation. */
#ifndef CELL_ALLEGRO_AUDIO_H
#define CELL_ALLEGRO_AUDIO_H
#include "native.h"
#define TRUE -1
#define ABS(x) ((x)<0?-(x):(x))
#define FALSE 0
#define ALLEGRO_DATE 19970000
#define DAT_ID(a,b,c,d) (((a)<<24)|((b)<<16)|((c)<<8)|(d))
#define PLAYMODE_LOOP 1
#define PLAYMODE_BIDIR 2
#define LOCK_FUNCTION(x)
#define LOCK_VARIABLE(x)
#define END_OF_FUNCTION(x)
#define _go32_dpmi_lock_data(a,b) 0
#define BPM_TO_TIMER(bpm) (1193181L * 60 / (bpm))
#define install_int_ex _install_int_ex
#define remove_int _remove_int
#define pack_fopen _pack_fopen
#define pack_fclose _pack_fclose
#define pack_getc _pack_getc
int allocate_voice(SAMPLE *s);
void deallocate_voice(int v);
void reallocate_voice(int v,SAMPLE *s);
void voice_start(int v);
void voice_stop(int v);
void voice_set_volume(int v,int value);
void voice_set_pan(int v,int value);
void voice_set_frequency(int v,int value);
void voice_set_playmode(int v,int mode);
void voice_set_position(int v,int pos);
int voice_get_position(int v);
void lock_sample(SAMPLE *s);
void _unlock_dpmi_data(void *p,int size);
void register_datafile_object(int type,void *(*load)(PACKFILE *,long),void (*destroy)(void *));
long pack_fread(void *buf,long size,PACKFILE *f);
long pack_fwrite(const void *buf,long size,PACKFILE *f);
int pack_fseek(PACKFILE *f,int offset);
int pack_putc(int c,PACKFILE *f);
#endif
