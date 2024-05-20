#include <stdio.h>
#include <string.h>
#include <locale.h>

#include "songbook.h"

int main(int argc, char **argv)
{
    int i, standalone;
   
    char file_input[PATH_SIZE] = "", file_output[PATH_SIZE] = "";

    FILE *fd_in, *fd_out;
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

    songbook_render(fd_in, fd_out, type, standalone);

    fclose(fd_in);
    fclose(fd_out);
    
    return 0;
}
