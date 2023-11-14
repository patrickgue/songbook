#include <stdio.h>
#include <string.h>

#include "songbook.h"

#define DEBUG

#define BUFF_SIZE 2048


int main(int argc, char **argv)
{
    int i, is_only_chords = 0;
    char buffer[BUFF_SIZE],
        chords_buffer[BUFF_SIZE] = "",
        text_buffer[BUFF_SIZE] = "",
        current_section_name[BUFF_SIZE],
        filename[BUFF_SIZE];

    FILE *fd;
    struct s_chord_text chords[CHORD_ITEMS_MAX];

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
            songbook_build_chord_list(chords, chords_buffer, text_buffer);
            strcpy(chords_buffer, "");
            strcpy(text_buffer, "");
            is_only_chords = 0;
        }
        i++;
    }

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
