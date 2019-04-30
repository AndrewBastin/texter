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

/* Returns a new string removed from a string */
char *deleteAtIndex(char *str, int startIndex, int endIndex) {
   size_t len = strlen(str);

   char *resp = malloc(((len - ((endIndex - startIndex) + 1)) + 1) * sizeof(char));

   for (int i = 0, k = 0; i <= len; i++) {
       if (!(i >= startIndex && i <= endIndex)) {
           resp[k++] = str[i];
       }
   }

   return resp;
}

/* Create a new string with the 2 strings concatenated at a certain index */
char *appendAtIndex(char *str, char *toConcat, int appendIndex) {

    size_t concLen = strlen(toConcat);
    size_t oldLen = strlen(str);

    char *result = malloc((oldLen + concLen + 1) * sizeof(char));

    memcpy(result, str, appendIndex * sizeof(char));
    memcpy(result + appendIndex, toConcat, concLen * sizeof(char));
    memcpy(result + appendIndex + concLen, str + appendIndex, (oldLen - appendIndex) * sizeof(char));

    result[oldLen + concLen] = '\0';

    return result;
}
