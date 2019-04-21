#ifndef EDITOR_H

#define EDITOR_H

struct Editor {
    
    /* Null if editor instance is not associated with a file, else the string of the file path */
    char *filename;
    /* 1 if the file is modified 0 if the file is not modified */
    short isModified;
    
    /* The line in the editor being currently edited */
    struct EditorLine *line;

    /* The horizontal position of the cursor (starts with 0) */
    int cursX;
    /* The vertical position of the cursor (starts with 0) */
    int cursY;

    /* The amount of characters scrolled (to the right) */
    int scrollX;
    /* The amount of characters scrolled (to the bottom) */
    int scrollY;

    /* The first line (the line with the prev NULL) (used to make deletes and stuff easier) */
    struct EditorLine *firstLine; 
    /* The line on the top on the current scroll config */
    struct EditorLine *scrollLine; 
    
    /* Number of lines the current editor instance holds */
    int lineCount;
};

struct Editor *editor_createEditorFromFile(char *filename);
struct Editor *editor_createBlankEditor();
void editor_run(struct Editor *editor);
void editor_freeEditor(struct Editor *editor);

#endif
