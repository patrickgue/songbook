#ifndef _render_h
#define _render_h

#include <wchar.h>

#include "types.h"

#include "songbook.h"


void render_init(FILE *, enum e_render_type, int);
void render_title(struct s_song_meta);
void render_section(wchar_t *);
void render_section_end(wchar_t *);
void render_line(struct s_chord_text *, int count);
void render_song_end();
#endif
