#ifndef _render_h
#define _render_h

#include "songbook.h"

enum e_render_type
{
    HTML,
    LATEX
};

void render_init(FILE *, enum e_render_type, int);
void render_title(struct s_song_meta);
void render_section(char *);
void render_line(struct s_chord_text *, int count);
void render_standalone_footer();
#endif
