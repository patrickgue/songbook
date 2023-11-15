#include "multibyte_substitution.h"
#include <string.h>
char _mb_table[128][2];
int _mb_count = 0;

void mb_subst(char *str, int max_len)
{
    int i, j, found_index;

    for (i = 0; str[i] != 0 && i < max_len; i++)
    {
        if (((unsigned char)str[i]) > 128)
        {
            found_index = -1;
            for (j = 0; j < _mb_count; j++)
            {
                if (str[i] == _mb_table[j][0] && str[i] == _mb_table[j][1])
                {
                    found_index = i;
                    break;
                }
            }
            if (found_index == -1)
            {
                _mb_table[_mb_count][0] = str[i];
                _mb_table[_mb_count][1] = str[i + 1];
                found_index = _mb_count;
                _mb_count += 1;
            }
            str[i] = found_index | 0b10000000;
            for (j = i + 1; str[j + 1] != 0; j++)
            {
                str[j] = str[j + 1];
            }
            str[j] = 0;
        }
    }
}

void mb_restore(char *str, int max_len)
{
    int i, j;
    for (i = 0; str[i] != 0 && i < max_len; i++)
    {
        if (((unsigned char)str[i]) >= 128)
        {
            for (j = strlen(str); j > i; j--)
            {
                str[j + 1] = str[j];
            }
            str[i + 1] = _mb_table[str[i] & 0b01111111][1];
            str[i]     = _mb_table[str[i] & 0b01111111][0];
            i++;
        }
    }
}
