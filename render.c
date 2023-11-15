#include "renderer_html.h"

#include "render.h"

enum e_render_type type;

void render_init(FILE *fd, enum e_render_type t, int standalone)
{
    type = t;

    if (type == HTML)
        render_html_init(fd, standalone);
}

void render_section(char *section)
{
    if (type == HTML)
        render_html_section(section);
}

void render_line(struct s_chord_text *chords, int count)
{
    if (type == HTML)
        render_html_line(chords, count);
}

void render_title(struct s_song_meta meta)
{
    if (type == HTML)
        render_html_title(meta);
}

void render_standalone_footer()
{
    if (type == HTML)
        render_html_standalone_footer();
}
