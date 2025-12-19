#include <string.h>
#include <ctype.h>

void trim(char *s)
{
    char *start = s;
    char *end;

    // leading
    while (*start && isspace((unsigned char)*start))
        start++;

    if (start != s)
        memmove(s, start, strlen(start) + 1);

    // trailing
    end = s + strlen(s) - 1;
    while (end >= s && isspace((unsigned char)*end))
        *end-- = '\0';
}

void trim_newline(char *s)
{
    s[strcspn(s, "\r\n")] = 0;
}
