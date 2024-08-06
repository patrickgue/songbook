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

void latex_section_title_transform(wchar_t *section, size_t len)
{
    int i;
    for (i = 0; i < wcslen(section) && i < len; i++)
    {
        if (section[i] >= L'a' && section[i] <= L'z')
        {
            section[i] = section[i] & (0xff - ('a'-'A'));
        }
    }
}

#define SECTION_LABEL_LEN 64

void render_latex_section(wchar_t *section)
{
    wchar_t sec[SECTION_LABEL_LEN];
    wcsncpy(sec, section, SECTION_LABEL_LEN);
    latex_section_title_transform(sec, SECTION_LABEL_LEN);
    // fwprintf(_latex_fd, L"\\begin{%ls}\n", section);
    fwprintf(_latex_fd, L"\\subsection*{%ls}\n", sec);
}

void render_latex_section_end(wchar_t *section)
{
    // fwprintf(_latex_fd, L"\\end{multicols}\n");
}

#define ESCAPE_CHORD_SIZE 6

void latex_chord_escape(wchar_t *dest, wchar_t *src)
{
    int i = 0, j = 0;
    while(src[i] != L'\0' && j < ESCAPE_CHORD_SIZE)
    {
        if (src[i] == L'#')
        {
            dest[j] = '\\';
            j++;
        }
        dest[j] = src[i];
        i++; j++;
    }
    dest[i] = 0;
}

void render_latex_line(struct s_chord_text *chords, int count)
{
    int i, has_text = 0;
    wchar_t section[BUFF_SIZE], chrds[ESCAPE_CHORD_SIZE];

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
            latex_chord_escape(chrds, chords[i].chord);
            if (has_text)
            {
                fwprintf(_latex_fd, L"\\chord{%ls}%ls", chrds, section);
            }
            else
            {
                fwprintf(_latex_fd, L"\\textbf{%ls}%ls ", chrds, section);
            }
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
    
    // fwprintf(_latex_fd, L"\\begin{song}[align-chords=l]{title=%ls", meta.song);

    fwprintf(_latex_fd, L"\\section*{%ls}\n", meta.song);

//    if (meta.capo != 0)
//        fwprintf(_latex_fd, L",capo=%d", meta.capo);
    //fwprintf(_latex_fd, L"}\n");
    if (wcslen(meta.artist) > 0)
        fwprintf(_latex_fd, L"\\textit{%ls}", meta.artist);

    if (wcslen(meta.artist) > 0 && meta.capo != 0)
        fwprintf(_latex_fd, L" - ");
    
    if (meta.capo != 0)
        fwprintf(_latex_fd, L"\\textit{Capo: %ls}\n", capo_str(meta.capo));
        
}

void render_latex_song_end()
{
    //fwprintf(_latex_fd, L"\\end{song}\n");
    fwprintf(_latex_fd, L"\\clearpage\n");
    if (_latex_standalone)
    {
        fwprintf(_latex_fd, L"\\end{document}\n");
    }
}
