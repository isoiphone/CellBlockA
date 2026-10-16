#ifndef CELL_JAILED_H
#define CELL_JAILED_H
#include "native.h"
#ifdef __cplusplus
extern "C" {
#endif
extern int jailed_tool, jailed_mouse_x, jailed_mouse_y, jailed_mouse_b;
void jailed_reset_map(void);
void jailed_draw_map(void);
void jailed_draw_tile(BITMAP*,int,int);
void jailed_screen_click(void);
BITMAP *jailed_mappic(void);
void _stretch_blit(BITMAP*,BITMAP*,int,int,int,int,int,int,int,int);
int jailed_main(int,char**);
#ifdef __cplusplus
}
#endif
#endif
