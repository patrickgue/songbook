#include <string.h>

#include "renderer_html_template.h"
#include "multibyte_substitution.h"

#include "renderer_html.h"

FILE *fd = NULL;
int standalone;

void render_html_init(FILE *out_fd, int s)
{
    fd = out_fd;
    standalone = s;
}

void render_html_section(char *section)
{
    char sec_cpy[BUFF_SIZE];
    strncpy(sec_cpy, section, BUFF_SIZE);
    sec_cpy[0] = sec_cpy[0] & 0b11011111;
    
    fprintf(fd, "<h3>%s</h3>\n", sec_cpy);
}

void render_html_line(struct s_chord_text *chords, int count)
{
    int i;
    char section[BUFF_SIZE];

    fprintf(fd, "<p class=\"line\">\n");
    for (i = 0; i < count; i++)
    {
        if (strlen(chords[i].chord) > 0)
        {
            fprintf(fd, "<span class=\"chord\">%s</span> ", chords[i].chord);
        }

        strncpy(section, chords[i].section, BUFF_SIZE);
        mb_restore(section, BUFF_SIZE);
        fprintf(fd, "<span class=\"section\" style=\"min-width: %dpx\">%s</span> \n", strlen(chords[i].chord) * 10, section);
    }
    fprintf(fd, "</p>\n");
}

void render_html_title(struct s_song_meta meta)
{
    char template_html_str[template_html_len + 1];
    if (standalone)
    {
        strncpy(template_html_str, template_html, template_html_len + 1);
        template_html_str[template_html_len] = 0;
        fprintf(fd, template_html_str, meta.song);
    }
    
    fprintf(fd, "<h2>%s</h2><p><i>%s", meta.song, meta.artist);
    if (meta.capo != 0)
    {
        fprintf(fd, ", Capo: %s", capo_str(meta.capo));
    }
    fprintf(fd, "</i></p>\n");
}

void render_html_standalone_footer()
{
    if (standalone)
        fprintf(fd, "</div>\n</body>\n</html>\n");
}
