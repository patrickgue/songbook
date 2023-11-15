#ifndef _songbook_h
#define _songbook_h

#include <stdint.h>

#define CHORD_TEXT_SIZE 1024
#define CHORD_ITEMS_MAX 64
#define BUFF_SIZE 2048

/*
#define A   0
#define AS  1
#define B   2
#define C   3
#define CS  4
#define D   5
#define DS  6
#define E   7
#define F   8
#define FS  9
#define G  10
#define GS 11*/

struct s_chord_text
{
    char chord[32];
    char section[CHORD_TEXT_SIZE];
};

struct s_song_meta
{
    char song[64];
    char artist[64];
    int capo;
};

int songbook_build_chord_list(struct s_chord_text *chords, char *chord_text, char *text);
void read_meta(char *line, struct s_song_meta *meta);
char *capo_str(int c);

#endif
