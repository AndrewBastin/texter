#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../util/cursesutil.h"
#include "../util/stringutil.h"
#include "line.h"
#include "editor.h"


/* Creates an editor instance for a given filename and load its contents, returns null if errors occur*/
struct Editor *editor_createEditorFromFile(char *filename) {

    struct Editor *editor = editor_createBlankEditor();
    editor->filename = filename;
    
    // Load file
    FILE *file = fopen(filename, "r");
    if (file == NULL) { // Error Handling
        editor_freeEditor(editor);
        return NULL;
    }

    // Read file character by character
    char c;
    while ((c = (char) getc(file)) != EOF) { // Read until EOF
        if (c == '\n') { // Go to next line on new line
            struct EditorLine *newLine = editor_createLine("", NULL, editor->line);
            
            editor->line->next = newLine;
            editor->line = newLine;
            editor->lineCount++;
        } else {
            char *cStr = charAsString(c);
            editor_appendToLine(editor->line, cStr, editor->line->len);

            free(cStr);
        }
    }

    fclose(file); // Close file

    editor->line = editor->firstLine; // Jump back to first line

    return editor;
}

/* Creates a new instance of an editor*/
struct Editor *editor_createBlankEditor() {

    struct Editor *editor = (struct Editor*) malloc(sizeof(struct Editor));

    editor->filename = NULL;
    editor->line = editor_createLine("", NULL, NULL);   // Create the first line
    editor->cursX = 0;
    editor->cursY = 0;
    editor->scrollX = 0;
    editor->scrollY = 0;
    editor->firstLine = editor->line;
    editor->scrollLine = editor->firstLine;
    editor->lineCount = 1;

    return editor;
}

/* Runs the given editor instance */
void editor_run(struct Editor *editor) {
    
    // Draw text from the beginning
    int ln = 0;
    for (struct EditorLine *line = editor->scrollLine; line != NULL && ln < curses_getScreenHeight() - 1; line = line->next, ln++) {
        if (editor->scrollX < line->len) {
            curses_drawText(ln, 0, line->str + editor->scrollX, line->len - editor->scrollX + 1);
        }
    }


    // Set cursor position
    curses_setCursorPos(editor->cursY - editor->scrollY, editor->cursX - editor->scrollX);

    int ch = curses_getch();

    switch (ch) {
        case CURSES_KEY_BACKSPACE:
            
            if (editor->cursX > 0) {                                                                    // There is text on the left to delete
                editor_deleteFromLine(editor->line, editor->cursX - 1, editor->cursX - 1);
                editor->cursX--;
            } else if (editor->cursX == 0 && editor->line->prev != NULL) {                              // There is no text on the left to delete, move line contents to the line above and destroy this line
                
                int oldPrevLen = editor->line->prev->len;                                               // Stored to move the cursor to correct place later

                editor_appendToLine(editor->line->prev, editor->line->str, editor->line->prev->len);    // Concat line contents

                if (editor->line->next != NULL) {                                                       // If there is a line below, set its previous line to the line above
                    editor->line->next->prev = editor->line->prev;
                }

                editor->line->prev->next = editor->line->next;                                          // Set the line before's next to the line below this line

                struct EditorLine *l = editor->line;                                                    // Temp pointer to store the value for deletion

                editor->line = editor->line->prev;

                editor_freeLine(l);                                                                     // Delete the line
                
                if (editor->cursY == editor->scrollY && editor->scrollLine->prev != NULL) {
                    editor->scrollY--;
                    editor->scrollLine = editor->scrollLine->prev;
                }
                editor->cursY--;                                                                        // Move the cursor up a line
                editor->cursX = oldPrevLen;                                                             // Set the horizontal cursor pos correctly
                
                // TODO : Update scroll
                if (editor->cursX - editor->scrollX > curses_getScreenWidth()) editor->scrollX = editor->cursX - curses_getScreenWidth() + 2;

                editor->lineCount--;
            }

            break;

        case CURSES_KEY_LEFT:

            if (editor->cursX > 0) {
                
                if (editor->cursX == editor->scrollX) editor->scrollX--;
                
                editor->cursX--;
            }
                
            break;

        case CURSES_KEY_RIGHT:

            if (editor->cursX < editor->line->len) {
                
                if (editor->cursX - editor->scrollX + 1 == curses_getScreenWidth()) editor->scrollX++;
                
                editor->cursX++;
            }

            break;

        case CURSES_KEY_UP:
            
            if (editor->cursY > 0) {

                if (editor->cursX > editor->line->prev->len) { // Clamp cursor x pos to the end of the previous line
                    editor->cursX = editor->line->prev->len;
                }

                editor->line = editor->line->prev;
                
                if (editor->cursY == editor->scrollY && editor->scrollLine->prev != NULL) {
                    editor->scrollY--;
                    editor->scrollLine = editor->scrollLine->prev;
                }

                editor->cursY--;
            }

            break;

        case CURSES_KEY_DOWN:
            
            if (editor->line->next != NULL) {

                if (editor->cursX > editor->line->next->len) { // Clamp cursor x pos to the end of the next line
                    editor->cursX = editor->line->next->len;
                }

                editor->line = editor->line->next;
                
                if (editor->cursY == editor->scrollY + curses_getScreenHeight() - 2) {
                    editor->scrollY++;
                    editor->scrollLine = editor->scrollLine->next;
                }
                editor->cursY++;
            }

            break;

        case '\n': {
            
            char *newLineContent = substring(editor->line->str, editor->cursX, editor->line->len);
            char *currLineContent = substring(editor->line->str, 0, editor->cursX);

            editor_updateLine(editor->line, currLineContent); // Splitting the text across lines

            struct EditorLine *newLine = editor_createLine(newLineContent, editor->line->next, editor->line);

            if (editor->line->next != NULL) {
                editor->line->next->prev = newLine;
            }
            editor->line->next = newLine;
            
            editor->line = editor->line->next;

            if (editor->cursY == editor->scrollY + curses_getScreenHeight() - 2) {
                editor->scrollY++;
                editor->scrollLine = editor->scrollLine->next;
            }
            editor->cursY++;
            editor->cursX = 0;
            editor->scrollX = 0;

            editor->lineCount++;
            
            free(newLineContent);
            free(currLineContent);
            
            break;
        }

        case CURSES_CHAR_CTRL('s'):
            
            if (editor->filename != NULL) {
                
                FILE *file = fopen(editor->filename, "w+");
                if (file != NULL) {
                    
                    short first = 1;
                    for (struct EditorLine *line = editor->firstLine; line != NULL; line = line->next) {
                        
                        // To put new line before all lines other than the first line
                        if (first) first = 0;
                        else fputc('\n', file);
                        
                        fputs(line->str, file);
                    }

                    fclose(file);
                }

            }

            break;
            
        default: {
            char *chrStr = charAsString(ch);
            editor_appendToLine(editor->line, chrStr, editor->cursX);

            // Scroll on typing into scroll end
            if (editor->line->len - editor->scrollX >= curses_getScreenWidth()) editor->scrollX++;
            
            editor->cursX++;
            
            free(chrStr);
            break;
        }
    }

    curses_clear();
}

/* Deallocates the given editor instance */
void editor_freeEditor(struct Editor *editor) {

    for (struct EditorLine *line = editor->firstLine; line != NULL;) {
        struct EditorLine *n = line; // Store a temp pointer to move line pointer safely

        line = line->next;
        editor_freeLine(n);
    }

    free(editor);
}