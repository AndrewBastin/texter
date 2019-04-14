#include <stdlib.h>
#include <string.h>

/* Returns a given char as a string */
char *charAsString(char ch) {
    char *str = malloc(2 * sizeof(char));
    str[0] = ch;
    str[1] = '\0';

    return str;
}

/* Returns a substring from the given part of the string */
char *substring(char *str, int startIndex, int endIndex) {
    char *s = malloc((endIndex - startIndex + 1) * sizeof(char)); // +1 for null terminator

    memcpy(s, str + startIndex, (endIndex - startIndex) * sizeof(char));
    s[endIndex - startIndex] = '\0';

    return s;
}