#include "renderer_latex_template.h"

#include "renderer_latex.h"

FILE *_latex_fd;
int _latex_standalone = 0;

void render_latex_init(FILE *f, int s)
{
    _latex_fd = f;
    _latex_standalone = s;
}

void render_latex_section(char *section)
{
    fprintf(_latex_fd, "\\begin{%s}\n", section);
}

void render_latex_section_end(char *section)
{
    fprintf(_latex_fd, "\\end{%s}\n", section);
}

void render_latex_line(struct s_chord_text *chords, int count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        fprintf(_latex_fd, "^{%s}%s ", chords[i].chord, chords[i].section);
    }
    fprintf(_latex_fd, "\\\\\n");
}

void render_latex_title(struct s_song_meta meta)
{
    if (_latex_standalone)
    {
        fwrite(template_tex, template_tex_len, 1, _latex_fd);
    }
    
    fprintf(_latex_fd, "\\begin{song}[align-chords=l]{title=%s", meta.song);
    if (meta.capo != 0)
        fprintf(_latex_fd, ",capo=%d", meta.capo);
    fprintf(_latex_fd, "}\n");
    if (meta.capo != 0)
        fprintf(_latex_fd, "\\capo\n");
        
}

void render_latex_song_end()
{
    fprintf(_latex_fd, "\\end{song}\n");
    if (_latex_standalone)
    {
        fprintf(_latex_fd, "\\end{document}\n");
    }
}
