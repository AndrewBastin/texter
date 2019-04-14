struct Editor {
    
    /* Null if editor instance is not associated with a file, else the string of the file path */
    char *filename;
    
    struct EditorLine *line;

    int cursX;
    int cursY;

    int scrollY;

    struct EditorLine *firstLine; // The first line (the line with the prev NULL) (used to make deletes and stuff easier)
    struct EditorLine *scrollLine; // The line on top on the current scroll config
    int lineCount;
};

struct Editor *editor_createEditorFromFile(char *filename);
struct Editor *editor_createBlankEditor();
void editor_run(struct Editor *editor);
void editor_freeEditor(struct Editor *editor);