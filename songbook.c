#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

#include "render.h"

#include "songbook.h"



int main(int argc, char **argv)
{
    int i, j, is_only_chords = 0, chords_count, standalone = 0;;
    wchar_t buffer[BUFF_SIZE],
        chords_buffer[BUFF_SIZE] = L"",
        text_buffer[BUFF_SIZE] = L"",
        current_section_name[BUFF_SIZE];
    char file_input[PATH_SIZE] = "", file_output[PATH_SIZE] = "", utf8_buffer[BUFF_SIZE];

    FILE *fd_in, *fd_out;
    struct s_chord_text chords[CHORD_ITEMS_MAX];
    struct s_song_meta meta;
    enum e_render_type type = NONE;

    setlocale(LC_ALL, "");


    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-s") == 0)
        {
            standalone = 1;
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            type = HTML;
        }
        else if (strcmp(argv[i], "-l") == 0)
        {
            type = LATEX;
        }
        else if (strcmp(argv[i], "-i") == 0 && i < argc - 1)
        {
            strncpy(file_input, argv[i + 1], PATH_SIZE);
        }
        else if (strcmp(argv[i], "-o") == 0 && i < argc - 1)
        {
            strncpy(file_output, argv[i + 1], PATH_SIZE);
        }
    }

    if (type == NONE)
    {
        fprintf(stderr, "No format provided\n");
        return -1;
    }

    if (strlen(file_input) > 0)
    {
        fd_in = fopen(file_input, "r");
        if (fd_in == NULL)
        {
            fprintf(stderr, "Unable to open file %s\n" ,file_input);
            return -1;
        }
    }
    else
    {
        fd_in = stdin;
    }

    if (strlen(file_output) > 0)
    {
        fd_out = fopen(file_output, "w");
        if (fd_out == NULL)
        {
            fprintf(stderr, "Unable to open file %s\n" ,file_output);
            return -1;
        }
    }
    else
    {
        fd_out = stdout;
    }


    render_init(fd_out, type, standalone);
    
    
    i = 1;
    
    while (fgets(utf8_buffer, BUFF_SIZE, fd_in) != NULL)
    {
        if (utf8_buffer == NULL || strlen(utf8_buffer) == 0)
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
                read_meta(buffer, &meta);
            }
        }

        if (buffer[0] == L'[')
        {
            if (wcslen(current_section_name) > 0)
            {
                render_section_end(current_section_name);
            }
            
            wcsncpy(current_section_name, (buffer + 1), BUFF_SIZE);
            current_section_name[wcslen(current_section_name) - 2] = 0;
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

        if (buffer[0] == L'>')
        {
            if (wcslen(text_buffer) > 0)
            {
                fprintf(stderr, "Warning: Line of text overwritten on line %d\n", i);
            }
            
            wcsncpy(text_buffer, buffer + 2, BUFF_SIZE);
            text_buffer[wcslen(text_buffer) - 1] = 0;
        }

        if (wcslen(chords_buffer) > 0 && (wcslen(text_buffer) > 0 || is_only_chords))
        {
            chords_count = songbook_build_chord_list(chords, chords_buffer, text_buffer);
            wcscpy(chords_buffer, L"");
            wcscpy(text_buffer, L"");
            is_only_chords = 0;

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

    fclose(fd_in);
    fclose(fd_out);
    
    return 0;
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

void read_meta(wchar_t *l, struct s_song_meta *meta)
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

wchar_t *capo_str(int c)
{
    switch (c)
    {
    case 1: return L"I";
    case 2: return L"II";
    case 3: return L"III";
    case 4: return L"IV";
    case 5: return L"V";
    case 6: return L"VI";
    case 7: return L"VII";
    case 8: return L"VIII";
    case 9: return L"IX";
    case 10: return L"X";
    case 11: return L"XI";
    }
}
