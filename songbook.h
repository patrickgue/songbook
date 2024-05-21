#ifndef _songbook_h
#define _songbook_h

#include <stdint.h>
#include <wchar.h>

#include "types.h"

#include "render.h"

#define PATH_SIZE 1024
#define CHORD_ITEMS_MAX 64

/*
#define A   0
#define AS  1
#define B   2
#define C   3
#define CS  4
#define D   5
#define DS  6
#define E   7
#define F   8
#define FS  9
#define G  10
#define GS 11*/

void songbook_render(FILE *fd_in, FILE *fd_out, enum e_render_type type, int standalone);
int songbook_build_chord_list(struct s_chord_text *chords, wchar_t *chord_text, wchar_t *text);
void read_meta(wchar_t *line, struct s_song_meta *meta);
wchar_t *capo_str(int c);

#endif
