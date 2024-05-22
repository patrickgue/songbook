#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <dirent.h>

#include "types.h"
#include "songbook.h"

#include "makebook.h"

int main(int argc, char **argv)
{
    int i;
    enum e_render_type type = NONE;
    char input_path[BUFF_SIZE] = ".", output_path[BUFF_SIZE] = "";
    FILE *output_file;

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

    output_file = fopen(output_path, "w");

    if (output_file == NULL)
    {
        die("unable to open output file");
    }

    makebook_traverse_tree(input_path, output_file, type);

    fclose(output_file);
    return 0;
}

void makebook_traverse_tree(char *path, FILE *out, enum e_render_type type)
{
    struct dirent *dir;
    char full_path[BUFF_SIZE];
    DIR *d = opendir(path);
    FILE *file_in;


    while ((dir = readdir(d)) != NULL)
    {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        if (dir->d_type & DT_DIR)
        {
            snprintf(full_path, BUFF_SIZE, "%s/%s", path, dir->d_name);
            makebook_traverse_tree(full_path, out, type);
        }
        else if (ends_with(dir->d_name, ".song"))
        {
            snprintf(full_path, BUFF_SIZE, "%s/%s", path, dir->d_name);
            file_in = fopen(full_path, "r");
            // TODO error handling
            songbook_render(file_in, out, type, BODY_ONLY);
            fclose(file_in);
        }
    }

    closedir(d);
}

int index_of(char *str, char *search)
{
    int i, j, found;

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == search[0])
        {
            found = 1;
            for (j = 0; j < strlen(search); j++)
            {
                if (str[i + j] != search[j])
                {
                    found = 0;
                    break;
                }
            }

            if (found)
            {
                return i;
            }
        }
    }
    return -1;
}

int ends_with(char *str, char *search)
{
    return (index_of(str, search) == strlen(str) - strlen(search));
}

void die(char *msg)
{
    fprintf(stderr, "Error, exiting: %s\n", msg);
    exit(1);
}
