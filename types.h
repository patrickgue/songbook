#ifndef _types_h
#define _types_h

#define CHORD_TEXT_SIZE 1024


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

enum e_render_type
{
    NONE,
    HTML,
    LATEX
};

#endif
