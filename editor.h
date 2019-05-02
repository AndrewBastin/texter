#include <termbox.h>

#ifndef EDITOR_H

#define EDITOR_H

struct Editor {
    
    /* Null if editor instance is not associated with a file, else the string of the file path */
    char *filename;
    /* 1 if the file exists in disk else not */
    short doesFileExist;
    /* 1 if the file is modified 0 if the file is not modified */
    short isModified;
    /* 1 if the file can't be written to (permissions) */
    short fileReadonly;

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

    /* Specifies whether a rerender should be done */
    short shouldRender;

    /* The first line (the line with the prev NULL) (used to make deletes and stuff easier) */
    struct EditorLine *firstLine; 
    /* The line on the top on the current scroll config */
    struct EditorLine *scrollLine; 
    
    /* Whether the editor is prompting or not */
    short isPrompting;
    /* What is the current prompt type (if open, else maybe garbage value) */
    int promptType;
    /* Stores char data for prompts to store state */
    char *promptLine;
    /* Prompt horizontal cursor position */
    int promptCursX;

    /* Number of lines the current editor instance holds */
    int lineCount;
};

/* Reads the event and updates the editor accordingly */
void editor_input(struct Editor *editor, struct tb_event *e);

/* Creates an editor instance for a given filename and load its contents, returns null if errors occur*/
struct Editor *editor_createEditorFromFile(char *filename);

/* Creates a new instance of an editor*/
struct Editor *editor_createBlankEditor();

/* Closes an open prompt session */
void editor_closePrompt(struct Editor *editor);

/* Saves the file in the editor */
void editor_savefile(struct Editor *editor);

/* Renders the editor */
void editor_render(struct Editor *editor);

/* Deallocates the given editor instance */
void editor_freeEditor(struct Editor *editor);

#endif
