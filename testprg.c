#include <assert.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>

#include "songbook.h"

void succ(char *msg)
{
    printf("[SUCCESS: %s]\n", msg);
}

void test_meta_read(void)
{
    wchar_t input[3][32] = {
        L"@song=Hello World\n",
        L"@artist=Artist\n",
        L"@capo=4\n"
    };
    int i;

    struct s_song_meta meta;

    meta_init(&meta);

    for (i = 0; i < 3; i++)
    {
        meta_read(input[i], &meta);
    }

    assert(meta.capo == 4);
    assert(wcsncmp(meta.artist, L"Artist", 32) == 0);
    assert(wcsncmp(meta.song, L"Hello World", 32) == 0);
}

void test_parse_chords_line()
{
    wchar_t *chord_txt = L"Am         C";
    wchar_t *text =      L"Hello, new World";
    struct s_chord_text chords[2];
    int count = songbook_build_chord_list(chords, chord_txt, text);

    assert(count == 2);
    assert(wcscmp(chords[0].chord, L"Am") == 0);
    assert(wcscmp(chords[1].chord, L"C") == 0);
    assert(wcscmp(chords[0].section, L"Hello, new ") == 0);
    assert(wcscmp(chords[1].section, L"World") == 0);
}

void test_render_html()
{
    char filename_in[] = "/tmp/testXXXXXX";
    char filename_out[] = "/tmp/testXXXXXX";

    FILE *tmp_in, *tmp_out;

    const char song_contents[] = "@song=Testsong\n\
@artist=Artist\n\
@maketitle\n\
\n\
[Verse]\n\
\n\
# Am         G\n\
> Hello, new World\n";

    const char song_rendered_html[] = "<div class=\"song\">\n\
<h2>Testsong</h2><p><i>Artist</i></p>\n\
<div class=\"song-content\"><div class=\"sec\">\n\
<h3>Verse</h3>\n\
<p class=\"line\">\n\
<span class=\"chord\">Am</span><span class=\"section\" style=\"min-width: 30px\">Hello, new </span><span class=\"chord\">G</span><span class=\"section\" style=\"min-width: 15px\">World</span></p>\n\
</div>\n\
</div></div>\n";

    char output_buffer[8192] = "", buff[1024];


    mkstemp(filename_in);
    mkstemp(filename_out);

    tmp_in = fopen(filename_in, "w");
    assert(tmp_in != NULL);

    fwrite(song_contents, sizeof(char), sizeof(song_contents), tmp_in); 
    
    fclose(tmp_in);
    tmp_in = fopen(filename_in, "r");
    assert(tmp_in != NULL);
    tmp_out = fopen(filename_out, "w");
    assert(tmp_out != NULL);

    songbook_render(tmp_in, tmp_out, HTML, 0);

    fclose(tmp_in);
    fclose(tmp_out);

    tmp_out = fopen(filename_out, "r");
    assert(tmp_out != NULL);

    while ((fgets(buff, 1024, tmp_out)) != NULL)
    {
        strcat(output_buffer, buff);
    }

    assert(strcmp(output_buffer, song_rendered_html) == 0);

    fclose(tmp_out);
}

void test_render_latex()
{
    char filename_in[] = "/tmp/testXXXXXX";
    char filename_out[] = "/tmp/testXXXXXX";

    FILE *tmp_in, *tmp_out;

    const char song_contents[] = "@song=Testsong\n\
@artist=Artist\n\
@maketitle\n\
\n\
[Verse]\n\
\n\
# Am         G\n\
> Hello, new World\n";

    const char song_rendered_latex[] = "\\section*{Testsong}\n\
\\textit{Artist}\\subsection*{VERSE}\n\
\\chord{Am}Hello, new \\chord{G}World\\\\\n\
\\clearpage\n";

    char output_buffer[8192] = "", buff[1024];


    mkstemp(filename_in);
    mkstemp(filename_out);

    tmp_in = fopen(filename_in, "w");
    assert(tmp_in != NULL);

    fwrite(song_contents, sizeof(char), sizeof(song_contents), tmp_in); 
    
    fclose(tmp_in);
    tmp_in = fopen(filename_in, "r");
    assert(tmp_in != NULL);
    tmp_out = fopen(filename_out, "w");
    assert(tmp_out != NULL);

    songbook_render(tmp_in, tmp_out, LATEX, 0);

    fclose(tmp_in);
    fclose(tmp_out);

    tmp_out = fopen(filename_out, "r");
    assert(tmp_out != NULL);

    while ((fgets(buff, 1024, tmp_out)) != NULL)
    {
        strcat(output_buffer, buff);
    }

    assert(strcmp(output_buffer, song_rendered_latex) == 0);

    fclose(tmp_out);
}

int main(int argc, char **argv)
{
    test_meta_read(); succ("meta_read");
    test_parse_chords_line(); succ("parse_chord_line");
    test_render_html(); succ("render_html");
    test_render_latex(); succ("render_latex");
    return 0;
}
