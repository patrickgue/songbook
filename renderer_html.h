#ifndef _renderer_html_h
#define _renderer_html_h

#include <wchar.h>
#include <stdio.h>

#include "songbook.h"


void render_html_init(FILE *, int);
void render_html_section(wchar_t *);
void render_html_line(struct s_chord_text *, int count);
void render_html_title(struct s_song_meta);
void render_html_standalone_footer();
#endif
