#ifndef _songbook_h
#define _songbook_h

#include <stdint.h>
#include <wchar.h>

#define PATH_SIZE 1024
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
    wchar_t chord[32];
    wchar_t section[CHORD_TEXT_SIZE];
};

struct s_song_meta
{
    wchar_t song[64];
    wchar_t artist[64];
    int capo;
};

int songbook_build_chord_list(struct s_chord_text *chords, wchar_t *chord_text, wchar_t *text);
void read_meta(wchar_t *line, struct s_song_meta *meta);
wchar_t *capo_str(int c);

#endif
