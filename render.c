#include "renderer_html.h"
#include "renderer_latex.h"

#include "render.h"

enum e_render_type type;

void render_type_not_supported(enum e_render_type t)
{
    printf("Render type %d not supported\n", t);
}

void render_init(FILE *fd, enum e_render_type t, int standalone)
{
    type = t;

    switch (type)
    {
    case HTML: render_html_init(fd, standalone); break;
    case LATEX: render_latex_init(fd, standalone); break;
    default: render_type_not_supported(type); break;
    }
}

void render_section(wchar_t *section)
{
    switch(type)
    {
    case HTML: render_html_section(section); break;
    case LATEX: render_latex_section(section); break;
    default: render_type_not_supported(type); break;        
    }
}

void render_line(struct s_chord_text *chords, int count)
{
    switch (type)
    {
    case HTML:  render_html_line(chords, count); break;
    case LATEX: render_latex_line(chords, count); break;
    default: render_type_not_supported(type); break;
    }
}

void render_title(struct s_song_meta meta)
{
    switch (type)
    {
    case HTML:  render_html_title(meta); break;
    case LATEX: render_latex_title(meta); break;
    default: render_type_not_supported(type); break;
    }
}

void render_song_end()
{
    switch (type)
    {
    case HTML: render_html_song_end(); break;
    case LATEX: render_latex_song_end(); break;
    default: render_type_not_supported(type); break;
    }
}

void render_section_end(wchar_t *section){
    switch (type)
    {
    case LATEX: render_latex_section_end(section); break;
    case HTML: render_html_section_end(); break;
    case NONE: render_type_not_supported(type);
    }
}
