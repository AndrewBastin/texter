#include <string.h>
#include <stdlib.h>

#include "line.h"

/* Creates a new editor line instance */
struct EditorLine *editor_createLine(char *contents, struct EditorLine *next, struct EditorLine *prev) {

    struct EditorLine *newLine = (struct EditorLine*) malloc(sizeof(struct EditorLine));    // Allocate memory for a new instance

    newLine->str = (char *) malloc(strlen(contents) * sizeof(char));                        // Allocate memory for the string
    strcpy(newLine->str, contents);                                                         // Copy the contents of the string
    newLine->len = strlen(contents);                                                        // Set the length of the line

    newLine->next = next;
    newLine->prev = prev;

    return newLine;
}

/* Updates the line content for a given instance */
void editor_updateLine(struct EditorLine *line, char *newContents) {

    free(line->str);                                                                // Deallocate existing line string
    
    line->str = (char *) malloc(strlen(newContents) * sizeof(char));                // Allocate new memory for the string
    strcpy(line->str, newContents);                                                 // Copy the contents of the string

    line->len = strlen(line->str);
}

/* Appends string to the line at appendIndex */
void editor_appendToLine(struct EditorLine *line, char *toConcat, int appendIndex) {

    char *oldStr = line->str;

    size_t concLen = strlen(toConcat);
    size_t oldLen = line->len;

    line->str = (char *) malloc((oldLen + concLen + 1) * sizeof(char));                                     // Allocate space for string (+1 for null terminator)

    memcpy(line->str, oldStr, appendIndex * sizeof(char));                                                  // Copy the first part of the old string                                                     
    memcpy(line->str + appendIndex, toConcat, concLen * sizeof(char));                                      // Copy the toConcat string
    memcpy(line->str + appendIndex + concLen, oldStr + appendIndex, (oldLen - appendIndex) * sizeof(char)); // Copy remaining old string
    
    line->str[oldLen + concLen] = '\0';                                                                     // Set the null terminator
    
    
    free(oldStr);                                                                                           // Free old string

    line->len = strlen(line->str);                                                                          // Recalculate string length
}

/* Delete certain character range from line (start and end range are inclusive) */
void editor_deleteFromLine(struct EditorLine *line, int startIndex, int endIndex) {

    char *oldStr = line->str;

    size_t oldLen = strlen(oldStr);

    line->str = (char *) malloc(((oldLen - ((endIndex - startIndex) + 1)) + 1) * sizeof(char));

    for (int i = 0, k = 0; i <= oldLen; i++) {          // <= oldLen for including the null terminator
        if (!(i >= startIndex && i <= endIndex)) {
            line->str[k++] = oldStr[i];
        }
    }

    free(oldStr);

    line->len = strlen(line->str);
}

/* Deallocates the editor line instance */
void editor_freeLine(struct EditorLine *line) {
    free(line->str);
    free(line);
}