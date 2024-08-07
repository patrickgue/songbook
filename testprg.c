#include <assert.h>
#include <stdio.h>
#include <wchar.h>

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

}

int main(int argc, char **argv)
{
    test_meta_read(); succ("meta_read");
    return 0;
}
