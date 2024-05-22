#ifndef makebook_h
#define makebook_h

#include "types.h"

void makebook_traverse_tree(char *, FILE *, enum e_render_type);

void makebook_template_copy_head(FILE *temp, FILE *fd);
void makebook_template_copy_foot(FILE *temp, FILE *fd);

int index_of(char *str, char *search);
int ends_with(char *, char *);

void die(char *);

#endif
