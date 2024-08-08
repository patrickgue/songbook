#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <dirent.h>
#include <locale.h>

#include <sys/stat.h>

#include "types.h"
#include "songbook.h"

#include "makebook.h"

#define DEBUG 1

char *template_head;
char *template_foot;

int main(int argc, char **argv)
{
    int i;
    enum e_render_type type = NONE;
    char input_path[BUFF_SIZE] = ".", output_path[BUFF_SIZE] = "", template_path[BUFF_SIZE] = "";
    FILE *output_file, *template_file;

    setlocale(LC_ALL, "");
    
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
                type = HTML;
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
        case 't':
            if (i < argc - 1)
                strncpy(template_path, argv[++i], BUFF_SIZE);
            else
                die("-t needs argument");
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

    template_file = fopen(template_path, "r");

    if (template_file != NULL)
    {
        makebook_template_copy_head(template_file, output_file);
    }

    makebook_traverse_tree(input_path, output_file, type);

    if (template_file != NULL)
    {
        makebook_template_copy_foot(template_file, output_file);
    }
    
    fclose(output_file);
    fclose(template_file);
    return 0;
}

void makebook_traverse_tree(char *path, FILE *out, enum e_render_type type)
{
    struct dirent *dir;
    char full_path[BUFF_SIZE], buff[BUFF_SIZE];
    DIR *d = opendir(path);
    FILE *file_in, *readme_file;
    int subdir_count = 0, songs_count = 0, i, is_dir;
    char (*subdir_paths)[PATH_SIZE] = malloc(0),
        (*songs_paths)[PATH_SIZE] = malloc(0);
    struct stat st;

    snprintf(full_path, BUFF_SIZE, "%s/readme.txt", path);
    readme_file = fopen(full_path, "r");

    if (type == HTML)
    {
        fwprintf(out, L"<div class=\"category\">\n");
    }
    
    if (readme_file != NULL)
    {
#ifdef DEBUG
        printf("Open file %s\n", full_path);
#endif

        if (type == HTML)
        {
            fwprintf(out, L"<p>");
        }
        else
        {
            fwprintf(out, L"{\\itshape ");
        }
        while (fgets(buff, BUFF_SIZE, readme_file) != NULL)
        {
            fwprintf(out, L"%s\n\n", buff);
#ifdef DEBUG
            printf("%s\n", buff);
#endif
        }
        if (type == HTML)
        {
            fwprintf(out, L"</p>");
        }
        else
        {
            fwprintf(out, L"}\n\\clearpage\n");
        }
        fclose(readme_file);
    }
    
    while ((dir = readdir(d)) != NULL)
    {
        /* skip everything starting with a '.' */
        if (dir->d_name[0] == '.')
            continue;

        snprintf(full_path, BUFF_SIZE, "%s/%s", path, dir->d_name);
        stat(full_path, &st);
        is_dir = S_ISDIR(st.st_mode);

        if (is_dir)
        {
            subdir_paths = realloc(subdir_paths, PATH_SIZE * (subdir_count + 1));
            strncpy(subdir_paths[subdir_count++], full_path, PATH_SIZE);
        }
        else if (index_of(dir->d_name, ".song") >= 0)
        {
            songs_paths = realloc(songs_paths, PATH_SIZE * (songs_count + 1));
            strncpy(songs_paths[songs_count++], full_path, PATH_SIZE);
        }
    }
    closedir(d);

    qsort(subdir_paths, subdir_count, PATH_SIZE, (int (*)(const void *, const void *)) strcmp);
    qsort(songs_paths, songs_count, PATH_SIZE, (int (*)(const void *, const void *)) strcmp);
    

#ifdef DEBUG
    printf("SUB DIRECTORIES\n");
#endif
    for (i = 0; i < subdir_count; i++)
    {
#ifdef DEBUG
        printf("TRAVERSE DIRECTORY %s\n", subdir_paths[i]);
#endif
        makebook_traverse_tree(subdir_paths[i], out, type);
    }

#ifdef DEBUG
    if (songs_count > 0)
        printf("PROCESS SONG FILES\n");
#endif    
    for (i = 0; i < songs_count; i++)
    {
#ifdef DEBUG
        printf("SONG FILE %s\n", songs_paths[i]);
#endif
        file_in = fopen(songs_paths[i], "r");
        // TODO error handling
        songbook_render(file_in, out, type, BODY_ONLY);
        fclose(file_in);
    }

    if (type == HTML)
    {
        fwprintf(out, L"</div><!-- /category -->");
    }

    free(subdir_paths);
    free(songs_paths);
}

void makebook_template_copy_head(FILE *temp, FILE *fd)
{
    char buff[BUFF_SIZE];

    while (fgets(buff, BUFF_SIZE, temp) != NULL)
    {
        if (strstr(buff, "<$body$>") != NULL)
            break;
        fwprintf(fd, L"%s", buff);
    }
}

void makebook_template_copy_foot(FILE *temp, FILE *fd)
{
    char buff[BUFF_SIZE];

    while (fgets(buff, BUFF_SIZE, temp) != NULL)
    {
        fwprintf(fd, L"%s", buff);
    }
}


int index_of(char *str, char *search)
{
    char *ptr = strstr(str, search);
    if (ptr == NULL)
    {
        return -1;
    }
    else
    {
        return str - search;
    }
}

int ends_with(char *str, char *search)
{
    return index_of(str, search) == (strlen(str) - strlen(search));
}

void die(char *msg)
{
    fprintf(stderr, "Error, exiting: %s\n", msg);
    exit(1);
}
