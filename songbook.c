#include <stdio.h>

#define BUFF_SIZE 2048

int main(int argc, char **argv)
{
    int i;
    char buffer[BUFF_SIZE],
        chords_buffer[BUFF_SIZE],
        text_buffer[BUFF_SIZE],
        current_section_name[BUFF_SIZE];

    i = 0;
    
    while (fgets(buffer, BUFF_SIZE, stdin) != NULL)
    {
        
        printf("%2d: %s", i++, buffer);
    }
    
    return 0;
}


void songbook_build_chord_list(struct s_chord_text *chords, char *chord_text, char *text)
{
    int i, j;

    for (i = 0, j = 0; i < strlen(chord_text); i++)
    {
        
    }
}
