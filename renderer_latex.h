#ifndef _renderer_latex_h
#define _renderer_latex_h

#include <wchar.h>
#include <stdio.h>

#include "songbook.h"


void render_latex_init(FILE *, int);
void render_latex_section(wchar_t *);
void render_latex_section_end(wchar_t *);
void render_latex_line(struct s_chord_text *, int count);
void render_latex_title(struct s_song_meta);
void render_latex_song_end();

#endif
