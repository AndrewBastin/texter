#include <stdlib.h>

struct EditorLine {
    
    char *str;
    size_t len;

    struct EditorLine *next;
    struct EditorLine *prev;
};

struct EditorLine *editor_createLine(char *contents, struct EditorLine *next, struct EditorLine *prev);
struct EditorLine *editor_updateLine(struct EditorLine *line, char *newContents);
struct EditorLine *editor_freeLine(struct EditorLine *line);
struct EditorLine *editor_appendToLine(struct EditorLine *line, char *toConcat, int appendIndex);
struct EditorLine *editor_deleteFromLine(struct EditorLine *line, int startIndex, int endIndex);