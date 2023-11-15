#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "render.h"

#include "songbook.h"

//#define DEBUG




int main(int argc, char **argv)
{
    int i, j, is_only_chords = 0, chords_count;
    char buffer[BUFF_SIZE],
        chords_buffer[BUFF_SIZE] = "",
        text_buffer[BUFF_SIZE] = "",
        current_section_name[BUFF_SIZE],
        filename[BUFF_SIZE];

    FILE *fd;
    struct s_chord_text chords[CHORD_ITEMS_MAX];
    struct s_song_meta meta;

    render_init(stdout, HTML, 1);
    
    strcpy(filename, "hello.song");
    fd = fopen(filename, "r");

    if (fd == NULL)
    {
        fprintf(stderr, "Unable to open file %s" ,filename);
        return -1;
    }
    
    i = 1;
    
    while (fgets(buffer, BUFF_SIZE, fd) != NULL)
    {
        if (buffer == NULL || strlen(buffer) == 0)
            continue;

        if (buffer[0] == '@')
        {
            if (strncmp(buffer, "@maketitle", strlen("@maketitle")) == 0)
            {
                render_title(meta);
            }
            else
            {
                read_meta(buffer, &meta);
            }
        }

        if (buffer[0] == '[')
        {
            strncpy(current_section_name, (buffer + 1), BUFF_SIZE);
            current_section_name[strlen(current_section_name) - 2] = 0;
            for (j = 0; j < strlen(current_section_name); j++)
            {
                if (current_section_name[j] >= 'A' && current_section_name[j] <= 'Z')
                    current_section_name[j] = current_section_name[j] | 0b1100000;
            }
#ifdef DEBUG
            printf("----------------\n%s\n----------------\n", current_section_name);
#endif
            render_section(current_section_name);
        }
        
        
        if (buffer[0] == '#')
        {
            if (strlen(chords_buffer) > 0)
            {
                fprintf(stderr, "Warning: Line of chords overwritten on line %d\n", i);
            }
            
            strncpy(chords_buffer, buffer + 2, BUFF_SIZE);
            chords_buffer[strlen(chords_buffer) - 1] = 0;
        }

        if (buffer[0] == '#' && buffer[1] == '#')
        {
            strncpy(chords_buffer, buffer + 3, BUFF_SIZE);
            chords_buffer[strlen(chords_buffer) - 1] = 0;
            is_only_chords = 1;
        }

        if (buffer[0] == '>')
        {
            if (strlen(text_buffer) > 0)
            {
                fprintf(stderr, "Warning: Line of text overwritten on line %d\n", i);
            }
            
            strncpy(text_buffer, buffer + 2, BUFF_SIZE);
            text_buffer[strlen(text_buffer) - 1] = 0;
        }

        if (strlen(chords_buffer) > 0 && (strlen(text_buffer) > 0 || is_only_chords))
        {
            chords_count = songbook_build_chord_list(chords, chords_buffer, text_buffer);
            strcpy(chords_buffer, "");
            strcpy(text_buffer, "");
            is_only_chords = 0;

            render_line(chords, chords_count);
        }
        i++;
    }
    render_standalone_footer();

    fclose(fd);
    
    return 0;
}


int songbook_build_chord_list(struct s_chord_text *chords, char *chord_text, char *text)
{
    int i, j, k, indexes[CHORD_ITEMS_MAX], is_inside_chord = 0;
    j = 0;

#ifdef DEBUG
    printf("Chords: '%s'\nText:   '%s'\n", chord_text, text);
#endif
    if (chord_text[0] == ' ')
    {
        indexes[j] = 0;
        j++;
    }

    for (i = 0; i < strlen(chord_text); i++)
    {
        if (is_inside_chord && chord_text[i] == ' ')
        {
            is_inside_chord = 0;
        }
        else if (!is_inside_chord && chord_text[i] != ' ')
        {
            is_inside_chord = 1;
            indexes[j++] = i;
        }
    }
    indexes[j] = -1;

    
    for (i = 0; i < j && indexes[i] != -1; i++)
    {
        strncpy(chords[i].chord, chord_text + indexes[i], 32);
        k = 0;
        while (chords[i].chord[k] != ' ')
            k++;
        chords[i].chord[k] = 0;
        if (indexes[i] < strlen(text))
        {
            strncpy(chords[i].section, text + indexes[i], CHORD_TEXT_SIZE);
            if (i < j - 1)
            {
                chords[i].section[indexes[i + 1] - indexes[i]] = 0;
            }
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

void read_meta(char *l, struct s_song_meta *meta)
{
    char key[64], line[64];
    int i = 0;
    strncpy(line, l, strlen(l) - 1);
    strncpy(key, line + 1, 64);
    while (key[i] != '=')
        i++;
    key[i] = 0;

    if (strncmp(key, "song", 64) == 0)
    {
        strncpy(meta->song, line + (i + 2), 64);
    }
    else if (strncmp(key, "artist", 64) == 0)
    {
        strncpy(meta->artist, line + (i + 2), 64);
    }
    else if (strncmp(key, "capo", 64) == 0)
    {
        meta->capo = atoi((const char *)(line + (i + 2)));
    }
}

char *capo_str(int c)
{
    switch (c)
    {
    case 1: return "I";
    case 2: return "II";
    case 3: return "III";
    case 4: return "IV";
    case 5: return "V";
    case 6: return "VI";
    case 7: return "VII";
    case 8: return "VIII";
    case 9: return "IX";
    case 10: return "X";
    case 11: return "XI";
    }
}
