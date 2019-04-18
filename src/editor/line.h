#include <stdlib.h>

struct EditorLine {

    /* The string representing the contents of the given line*/    
    char *str;

    /* Length of the given line (updated by editor_updateLine) */
    size_t len;

    /* The line after the given line (usually the line below) */
    struct EditorLine *next;
    /* The line before the given line (usually the line above) */
    struct EditorLine *prev;
};

struct EditorLine *editor_createLine(char *contents, struct EditorLine *next, struct EditorLine *prev);
void editor_updateLine(struct EditorLine *line, char *newContents);
void editor_freeLine(struct EditorLine *line);
void editor_appendToLine(struct EditorLine *line, char *toConcat, int appendIndex);
void editor_deleteFromLine(struct EditorLine *line, int startIndex, int endIndex);