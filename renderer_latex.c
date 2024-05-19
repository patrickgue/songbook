#include <string.h>


#include "renderer_latex_template.h"
#include "renderer_latex.h"

FILE *_latex_fd;
int _latex_standalone = 0;

void render_latex_init(FILE *f, int s)
{
    _latex_fd = f;
    _latex_standalone = s;
}

void render_latex_section(wchar_t *section)
{
    fwprintf(_latex_fd, L"\\begin{%ls}\n", section);
}

void render_latex_section_end(wchar_t *section)
{
    fwprintf(_latex_fd, L"\\end{%ls}\n", section);
}

void render_latex_line(struct s_chord_text *chords, int count)
{
    int i, has_text = 0;
    wchar_t section[BUFF_SIZE];

    for (i = 0; i < count; i++)
    {
        if (wcslen(chords[i].section) > 0)
        {
            has_text = 1;
            break;
        }
    }

    for (i = 0; i < count; i++)
    {
        wcsncpy(section, chords[i].section, BUFF_SIZE);

        if (wcslen(chords[i].chord) > 0)
        {
            fwprintf(_latex_fd, L"%c{%ls}%ls ", has_text ? '^' : '_', chords[i].chord, section);
        }
        else
        {
            fwprintf(_latex_fd, L"%ls ", section);
        }
    }
    fwprintf(_latex_fd, L"\\\\\n");
}

void render_latex_title(struct s_song_meta meta)
{
    if (_latex_standalone)
    {
        template_tex[template_tex_len - 1] = 0;
        fwprintf(_latex_fd, L"%hs", template_tex);
    }
    
    fwprintf(_latex_fd, L"\\begin{song}[align-chords=l]{title=%ls", meta.song);
    if (meta.capo != 0)
        fwprintf(_latex_fd, L",capo=%d", meta.capo);
    fwprintf(_latex_fd, L"}\n");
    if (meta.capo != 0)
        fwprintf(_latex_fd, L"\\capo\n");
        
}

void render_latex_song_end()
{
    fwprintf(_latex_fd, L"\\end{song}\n");
    if (_latex_standalone)
    {
        fwprintf(_latex_fd, L"\\end{document}\n");
    }
}
