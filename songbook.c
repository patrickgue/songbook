#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

#include "render.h"

#include "songbook.h"

void songbook_render(FILE *fd_in, FILE *fd_out, enum e_render_type type, int standalone)
{
    int i, j, is_only_chords = 0, is_only_text = 0, chords_count;
    char utf8_buffer[BUFF_SIZE];
    wchar_t buffer[BUFF_SIZE],
        chords_buffer[BUFF_SIZE] = L"",
        text_buffer[BUFF_SIZE] = L"",
        current_section_name[BUFF_SIZE] = L"";
    struct s_chord_text chords[CHORD_ITEMS_MAX] = {};
    struct s_song_meta meta;

    meta_init(&meta);
    render_init(fd_out, type, standalone);
    
    i = 1;

    while (fgets(utf8_buffer, BUFF_SIZE, fd_in) != NULL)
    {
        if (strlen(utf8_buffer) == 0 || utf8_buffer[0] == '\n')
            continue;

        mbstowcs(buffer, utf8_buffer, BUFF_SIZE);
        
        if (buffer[0] == L'@')
        {
            if (wcsncmp(buffer, L"@maketitle", 10) == 0)
            {
                render_title(meta);
            }
            else
            {
                meta_read(buffer, &meta);
            }
        }

        if (buffer[0] == L'[')
        {
            if (wcslen(current_section_name) > 0)
            {
                render_section_end(current_section_name);
            }
            
            wcsncpy(current_section_name, (buffer + 1), BUFF_SIZE);
            for (j = wcslen(current_section_name); j > 0; j--)
            {
                if (current_section_name[j] == L']')
                    current_section_name[j] = 0;
            }

            for (j = 0; j < wcslen(current_section_name); j++)
            {
                if (current_section_name[j] >= L'A' && current_section_name[j] <= L'Z')
                    current_section_name[j] = current_section_name[j] | 0b1100000;
            }
#ifdef DEBUG
            printf("----------------\n%s\n----------------\n", current_section_name);
#endif
            render_section(current_section_name);
        }
        
        
        if (buffer[0] == L'#')
        {
            if (wcslen(chords_buffer) > 0)
            {
                fprintf(stderr, "Warning: Line of chords overwritten on line %d\n", i);
            }
            
            wcsncpy(chords_buffer, buffer + 2, BUFF_SIZE);
            chords_buffer[wcslen(chords_buffer) - 1] = 0;
        }

        if (buffer[0] == L'#' && buffer[1] == L'#')
        {
            wcsncpy(chords_buffer, buffer + 3, BUFF_SIZE);
            chords_buffer[wcslen(chords_buffer) - 1] = 0;
            is_only_chords = 1;
        }

        if (buffer[0] == L'>' && buffer[1] == L'>')
        {
            wcsncpy(text_buffer, buffer + 3, BUFF_SIZE);
            text_buffer[wcslen(text_buffer) - 1] = 0;
            wcscpy(chords_buffer, L"");
            is_only_text = 1;
        }
        else if (buffer[0] == L'>')
        {
            if (wcslen(text_buffer) > 0)
            {
                fprintf(stderr, "Warning: Line of text overwritten on line %d\n", i);
            }
            
            wcsncpy(text_buffer, buffer + 2, BUFF_SIZE);
            text_buffer[wcslen(text_buffer) - 1] = 0;
        }

        if ((wcslen(chords_buffer) > 0 || is_only_text) && (wcslen(text_buffer) > 0 || is_only_chords))
        {
            if (is_only_text)
            {
                chords_count = songbook_build_text_only_line(chords, text_buffer);
            }
            else
            {
                chords_count = songbook_build_chord_list(chords, chords_buffer, text_buffer);
            }

            wcscpy(chords_buffer, L"");
            wcscpy(text_buffer, L"");
            is_only_chords = 0;
            is_only_text = 0;

            render_line(chords, chords_count);
        }
        i++;
    }

    if (ferror(fd_in) != 0)
    {
        perror("ERROR");
    }
    
    render_section_end(current_section_name);
    render_song_end();
}

int songbook_build_text_only_line(struct s_chord_text *chords, wchar_t *text)
{
    wcscpy(chords->chord, L"");
    wcsncpy(chords->section, text, CHORD_TEXT_SIZE);

    return 1;
}

int songbook_build_chord_list(struct s_chord_text *chords, wchar_t *chord_text, wchar_t *text)
{
    int i, j, k, indexes[CHORD_ITEMS_MAX], is_inside_chord = 0;
    j = 0;
  
#ifdef DEBUG
    wprintf(L"Chords: '%ls'\nText:   '%ls'\n", chord_text, text);
#endif
    if (chord_text[0] == ' ')
    {
        indexes[j] = 0;
        j++;
    }

    for (i = 0; i < wcslen(chord_text); i++)
    {
        if (is_inside_chord && chord_text[i] == L' ')
        {
            is_inside_chord = 0;
        }
        else if (!is_inside_chord && chord_text[i] != L' ')
        {
            is_inside_chord = 1;
            indexes[j++] = i;
        }
    }
    indexes[j] = -1;

    
    for (i = 0; i < j && indexes[i] != -1; i++)
    {
        wcsncpy(chords[i].chord, chord_text + indexes[i], 32);
        k = 0;
        while (chords[i].chord[k] != L' ' && chords[i].chord[k] != L'\0')
        {
            k++;
        }
        chords[i].chord[k] = 0;
        if (indexes[i] < wcslen(text))
        {
            wcsncpy(chords[i].section, text + indexes[i], CHORD_TEXT_SIZE);
            if (i < j - 1)
            {
                chords[i].section[indexes[i + 1] - indexes[i]] = 0;
            }
        }
        else
        {
            wcscpy(chords[i].section, L"");
        }
#ifdef DEBUG
        printf("%d: '%s': '%s'\n", i, chords[i].chord, chords[i].section);
#endif
    }
#ifdef DEBUG
    printf("\n");
#endif

    return j;
}

void meta_read(wchar_t *l, struct s_song_meta *meta)
{
    wchar_t key[64] = L"", line[64] = L"";
    int i = 0;
    wcsncpy(line, l, wcslen(l) - 1);
    wcsncpy(key, line + 1, 64);
    while (key[i] != L'=')
        i++;
    key[i] = 0;

    if (wcsncmp(key, L"song", 64) == 0)
    {
        wcsncpy(meta->song, line + (i + 2), 64);
    }
    else if (wcsncmp(key, L"artist", 64) == 0)
    {
        wcsncpy(meta->artist, line + (i + 2), 64);
    }
    else if (wcsncmp(key, L"capo", 64) == 0)
    {
        meta->capo = (int) wcstol((line + (i + 2)), NULL, 10);
    }
}

void meta_init(struct s_song_meta *meta)
{
    wcscpy(meta->artist, L"");
    wcscpy(meta->song, L"");
    meta->capo = 0;
}

wchar_t capos[12][5] = {
    L"I",
    L"II",
    L"III",
    L"IV",
    L"V",
    L"VI",
    L"VII",
    L"VIII",
    L"IX",
    L"X",
    L"XI"
};

wchar_t *capo_str(int c)
{
    if (c >= 1 && c <= 12)
        return capos[c];

    return L"";
}
