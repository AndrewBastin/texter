#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../util/cursesutil.h"
#include "../util/stringutil.h"
#include "line.h"
#include "editor.h"

/* Creates a new instance of an editor*/
struct Editor *editor_createBlankEditor() {

    struct Editor *editor = (struct Editor*) malloc(sizeof(struct Editor));

    editor->filename = NULL;
    editor->line = editor_createLine("", NULL, NULL);   // Create the first line
    editor->cursX = 0;
    editor->cursY = 0;
    editor->firstLine = editor->line;
    editor->lineCount = 1;

    return editor;
}

/* Runs the given editor instance */
void editor_run(struct Editor *editor) {
    
    // Set cursor position
    curses_setCursorPos(editor->cursY, editor->cursX);

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
                
                editor->cursY--;                                                                        // Move the cursor up a line
                editor->cursX = oldPrevLen;                                                             // Set the horizontal cursor pos correctly
                
                editor->lineCount--;
            }

            break;

        case CURSES_KEY_LEFT:
                
            if (editor->cursX > 0) {
                editor->cursX--;
            }
                
            break;

        case CURSES_KEY_RIGHT:

            if (editor->cursX < editor->line->len) {
                editor->cursX++;
            }

            break;

        case CURSES_KEY_UP:
            
            if (editor->cursY > 0) {

                if (editor->cursX > editor->line->prev->len) { // Clamp cursor x pos to the end of the previous line
                    editor->cursX = editor->line->prev->len;
                }

                editor->line = editor->line->prev;
                editor->cursY--;
            }

            break;

        case CURSES_KEY_DOWN:
            
            if (editor->line->next != NULL) {

                if (editor->cursX > editor->line->next->len) { // Clamp cursor x pos to the end of the next line
                    editor->cursX = editor->line->next->len;
                }

                editor->line = editor->line->next;
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
            editor->cursY++;
            editor->cursX = 0;

            editor->lineCount++;
            
            free(newLineContent);
            free(currLineContent);
            
            break;
        }
            
        default: {
            char *chrStr = charAsString(ch);
            editor_appendToLine(editor->line, chrStr, editor->cursX);
            editor->cursX++;
            
            free(chrStr);
            break;
        }
    }

    curses_clear();

    // Draw text from the beginning
    int ln = 0;
    for (struct EditorLine *line = editor->firstLine; line != NULL; line = line->next) {
        curses_drawText(ln++, 0, line->str);
    }
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