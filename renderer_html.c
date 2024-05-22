#include <string.h>

#include "renderer_html_template.h"

#include "renderer_html.h"

FILE *fd = NULL;
int standalone;

void render_html_init(FILE *out_fd, int s)
{
    fd = out_fd;
    standalone = s;
}

void render_html_section(wchar_t *section)
{
    wchar_t sec_cpy[BUFF_SIZE];
    wcsncpy(sec_cpy, section, BUFF_SIZE);
    sec_cpy[0] = sec_cpy[0] & 0b11011111;
    
    fwprintf(fd, L"<div class=\"sec\">\n<h3>%ls</h3>\n", sec_cpy);
}

void render_html_section_end()
{
    fwprintf(fd, L"</div>\n");
}

void render_html_line(struct s_chord_text *chords, int count)
{
    int i;
    wchar_t section[BUFF_SIZE];

    fwprintf(fd, L"<p class=\"line\">\n");
    for (i = 0; i < count; i++)
    {
        if (wcslen(chords[i].chord) > 0)
        {
            fwprintf(fd, L"<span class=\"chord\">%ls</span>", chords[i].chord);
        }

        wcsncpy(section, chords[i].section, BUFF_SIZE);
        fwprintf(fd, L"<span class=\"section\" style=\"min-width: %dpx\">%ls</span>", wcslen(chords[i].chord) * 10, section);
    }
    fwprintf(fd, L"</p>\n");
}

void render_html_title(struct s_song_meta meta)
{
    char template_html_str[template_html_len + 1];
    wchar_t w_template[template_html_len + 1];
    if (standalone)
    {
        strncpy(template_html_str, (const char*) template_html, template_html_len + 1);
        template_html_str[template_html_len] = 0;
        swprintf(w_template, template_html_len + 1, L"%hs", template_html_str);
        fwprintf(fd, w_template, meta.song);
    }
    
    fwprintf(fd, L"<div class=\"song\">\n<h2>%ls</h2><p><i>%ls", meta.song, meta.artist);
    if (meta.capo != 0)
    {
        fwprintf(fd, L" Capo: %ls", capo_str(meta.capo));
    }
    fwprintf(fd, L"</i></p>\n<div class=\"song-content\">");
}

void render_html_song_end()
{
    fwprintf(fd, L"</div></div>\n");
    if (standalone)
        render_html_standalone_footer();
}

void render_html_standalone_footer()
{
    fwprintf(fd, L"</div>\n</body>\n</html>\n");
}
