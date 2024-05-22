#ifndef makebook_h
#define makebook_h

#include "types.h"

void makebook_traverse_tree(char *, FILE *, enum e_render_type);
int index_of(char *str, char *search);
void die(char *);
int ends_with(char *, char *);


#endif
