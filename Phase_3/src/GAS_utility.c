#include "GAS_utility.h"

size_t mlg10(size_t u)
{
    size_t n = 0;
    do
    {
        u /= 10;
        n += 1;
    } while (u);
    return n;
}

void reverse_str(char *s, size_t len)
{
    for (size_t i = 0; i < len / 2; i++)
    {
        char t = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = t;
    }
}
