#include <stdio.h>
#include <string.h>

#include "songbook.h"

#define BUFF_SIZE 2048


int main(int argc, char **argv)
{
    int i, is_only_chords = 0;
    char buffer[BUFF_SIZE],
        chords_buffer[BUFF_SIZE],
        text_buffer[BUFF_SIZE],
        current_section_name[BUFF_SIZE];

    FILE *fd = fopen("test.song", "r");

    struct s_chord_text chords[CHORD_ITEMS_MAX];

    i = 0;
    
    while (fgets(buffer, BUFF_SIZE, fd) != NULL)
    {
        if (buffer[0] == '#')
        {
            strncpy(chords_buffer, buffer + 2, BUFF_SIZE);
        }

        if (buffer[0] == '#' && buffer[1] == '#')
        {
            strncpy(chords_buffer, buffer + 3, BUFF_SIZE);
            is_only_chords = 1;
        }

        if (buffer[0] == '>')
        {
            strncpy(text_buffer, buffer + 2, BUFF_SIZE);
        }

        if (strlen(chords_buffer) > 0 && (strlen(text_buffer) > 0 || is_only_chords))
        {
            songbook_build_chord_list(chords, chords_buffer, text_buffer);
            strcpy(chords_buffer, "");
            strcpy(text_buffer, "");
            is_only_chords = 0;
        }
    }

    fclose(fd);
    
    return 0;
}


int songbook_build_chord_list(struct s_chord_text *chords, char *chord_text, char *text)
{
    int i, j, indexes[CHORD_ITEMS_MAX], is_inside_chord = 0;
    j = 0;

    if (chord_text[0] == ' ')
    {
        indexes[j] = 0;
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
        strncpy(chords[j].chord, chord_text + indexes[i], 32);
        chords[i].chord[indexes[i + 1] - indexes[i]] = 0;
        printf("%s / %s ", chord_text + indexes[j], chords[j].chord);
    }
    printf("\n");

    return j;
}
