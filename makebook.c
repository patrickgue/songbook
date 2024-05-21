#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "types.h"
#include "songbook.h"

#include "makebook.h"

int main(int argc, char **argv)
{
    int i;
    enum e_render_type type = NONE;
    char input_path[BUFF_SIZE] = ".", output_path[BUFF_SIZE] = "";

    for (i = 1; i < argc; i++)
    {
        switch(argv[i][1])
        {
        case 'l':
            if (type == NONE)
                type = LATEX;
            else
                die("Only one output format can be specified");
            break;
        case 'h':
            if (type == NONE)
                type = LATEX;
            else
                die("Only one output format can be specified");
            break;
        case 'p':
            if (i < argc - 1)
                strncpy(input_path, argv[++i], BUFF_SIZE);
            else
                die("-p needs argument");
            break;
        case 'o':
            if (i < argc - 1)
                strncpy(output_path, argv[++i], BUFF_SIZE);
            else
                die("-p needs argument");
            break;
        default:
            fprintf(stderr, "unknown argument: %s\n", argv[i]);
            die("unknown argument");
        }
    }

    if (type == NONE)
    {
        die("Please provide format");
    }

    printf("Type: %s\nOutput Path: %s\nInput Path: %s\n", type == LATEX ? "LATEX" : "HTML", output_path, input_path);
    return 0;
}


void die(char *msg)
{
    fprintf(stderr, "Error, exiting: %s\n", msg);
    exit(1);
}
