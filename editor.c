#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <termbox.h>

#include "renderer.h"
#include "stringutil.h"
#include "fileutil.h"
#include "prompt.h"
#include "line.h"
#include "editor.h"


void editor_closePrompt(struct Editor *editor) {
    editor->isPrompting = 0;
    editor->promptType = -1;
    free(editor->promptLine);
    editor->promptLine = NULL;
    editor->promptCursX = 0;
}

void editor_startPrompt(struct Editor *editor, int editorType) {
    editor->isPrompting = 1;
    editor->promptType = editorType;
    editor->promptLine = malloc(sizeof(char));
    strcpy(editor->promptLine, "");
    editor->promptCursX = 0;
}

struct Editor *editor_createEditorFromFile(char *filename) {

    struct Editor *editor = editor_createBlankEditor();
    editor->filename = filename;
    editor->doesFileExist = file_exists(filename);

    if (editor->doesFileExist) {
            
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
    }

    return editor;
}

struct Editor *editor_createBlankEditor() {

    struct Editor *editor = (struct Editor*) malloc(sizeof(struct Editor));

    editor->filename = NULL;
    editor->line = editor_createLine("", NULL, NULL);   // Create the first line
    editor->isModified = 0;
    editor->cursX = 0;
    editor->cursY = 0;
    editor->scrollX = 0;
    editor->scrollY = 0;
    editor->shouldRender = 1;
    editor->firstLine = editor->line;
    editor->scrollLine = editor->firstLine;
    editor->lineCount = 1;
    editor_closePrompt(editor);

    return editor;
}

void editor_savefile(struct Editor *editor) { 
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

        editor->isModified = 0;
    }
}

/* Renders the editor */
void editor_render(struct Editor *editor) {
    int ln = 0;
    if (editor->shouldRender == 0) return;

    for (struct EditorLine *line = editor->scrollLine; line != NULL && ln < renderer_getScreenHeight() - 1; line = line->next, ln++) {
        if (editor->scrollX < line->len) {
            int pos = 0;
            for (char *x = line->str + editor->scrollX; *x != '\0'; x++) {
                
                switch (*x) {
                    
                    // for now, let tabs be rendered as single characters to make cursor positioning easier
                    case '\t':
                        renderer_drawChar(ln, pos++, ' ');
                        break;

                    default:
                        renderer_drawChar(ln, pos++, *x);
                        break;

                }

            }
        }
    }

    renderer_setCursorPos(editor->cursY - editor->scrollY, editor->cursX - editor->scrollX);

    if (editor->isPrompting) {
        prompt_render(editor); 
    }

    editor->shouldRender = 0;
}

/* Use this function in place of just editor->cursY-- to handle horizontal scroll updates */
void editor_moveCursorUp(struct Editor *editor) {
    if (editor->cursY == editor->scrollY && editor->scrollLine->prev != NULL) {
        editor->scrollY--;
        editor->scrollLine = editor->scrollLine->prev;
    }
    editor->cursY--;
}

/* Use this function in place of just editor->cursY++ to handle horizontal scroll updates */
void editor_moveCursorDown(struct Editor *editor) {
    if (editor->cursY == editor->scrollY + renderer_getScreenHeight() - 2) {
        editor->scrollY++;
        editor->scrollLine = editor->scrollLine->next;
    }
    editor->cursY++;
}

void editor_input_backsapce(struct Editor *editor) {
    if (editor->cursX > 0) {                                                                    // There is text on the left to delete
        editor_deleteFromLine(editor->line, editor->cursX - 1, editor->cursX - 1);

        if (editor->cursX == editor->scrollX + 1 && editor->scrollX > 0) editor->scrollX--;

        editor->cursX--;
        editor->isModified = 1;
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
        
        editor_moveCursorUp(editor);
        editor->cursX = oldPrevLen;                                                             // Set the horizontal cursor pos correctly
        
        // Update scroll
        if (editor->cursX - editor->scrollX > renderer_getScreenWidth()) editor->scrollX = editor->cursX - renderer_getScreenWidth() + 2;

        editor->lineCount--;

        editor->isModified = 1;
    }
}

void editor_input(struct Editor *editor, struct tb_event *e) {
    editor->shouldRender = 1;

    // If prompting, pipe input
    if (editor->isPrompting) {
        prompt_input(editor, e);
        return;
    }

    // Block certain unimplemented keys
    if (e->key == TB_KEY_ESC) return;

    switch (e->key) {
        
        case TB_KEY_BACKSPACE2:
            editor_input_backsapce(editor);
            break;

        case TB_KEY_BACKSPACE:
            editor_input_backsapce(editor);
            break;
        
        case TB_KEY_ARROW_LEFT:
            
            if (editor->cursX > 0) {
                
                if (editor->cursX == editor->scrollX) editor->scrollX--;
                
                editor->cursX--;
            } else if (editor->cursX == 0 && editor->cursY > 0) {
                editor_moveCursorUp(editor);

                editor->line = editor->line->prev;	
                editor->cursX = editor->line->len;
                
                if (editor->cursX - editor->scrollX > renderer_getScreenWidth()) editor->scrollX = editor->cursX - renderer_getScreenWidth() + 2;
            }
                
            break;

        case TB_KEY_ARROW_RIGHT:

            if (editor->cursX < editor->line->len) {

                if (editor->cursX - editor->scrollX + 1 == renderer_getScreenWidth()) editor->scrollX++;
                
                editor->cursX++;
            } else if (editor->cursX == editor->line->len && editor->line->next != NULL) {

                editor_moveCursorDown(editor);
                editor->line = editor->line->next;
                editor->cursX = 0;
                editor->scrollX = 0;

            }

            break;

        case TB_KEY_ARROW_UP:
            
            if (editor->cursY > 0) {

                if (editor->cursX > editor->line->prev->len) { // Clamp cursor x pos to the end of the previous line
                    editor->cursX = editor->line->prev->len;
                }

                editor->line = editor->line->prev;
                
                editor_moveCursorUp(editor);
            }
            
            break;

        case TB_KEY_ARROW_DOWN:

            if (editor->line->next != NULL) {

                if (editor->cursX > editor->line->next->len) { // Clamp cursor x pos to the end of the next line
                    editor->cursX = editor->line->next->len;
                }

                editor->line = editor->line->next;
                
                editor_moveCursorDown(editor);
            }
            
            break;

        case TB_KEY_ENTER: {

            char *newLineContent = substring(editor->line->str, editor->cursX, editor->line->len);
            char *currLineContent = substring(editor->line->str, 0, editor->cursX);

            editor_updateLine(editor->line, currLineContent); // Splitting the text across lines

            struct EditorLine *newLine = editor_createLine(newLineContent, editor->line->next, editor->line);

            if (editor->line->next != NULL) {
                editor->line->next->prev = newLine;
            }
            editor->line->next = newLine;
            
            editor->line = editor->line->next;

            editor_moveCursorDown(editor);
            editor->cursX = 0;
            editor->scrollX = 0;

            editor->lineCount++;
            
            free(newLineContent);
            free(currLineContent);

            editor->isModified = 1;
            
            break;
        }

        case TB_KEY_TAB:
            editor_appendToLine(editor->line, "    ", editor->cursX);
            editor->cursX += 4;

            if (editor->cursX - editor->scrollX >= renderer_getScreenWidth()) editor->scrollX += 4;
            break;
        
        case TB_KEY_CTRL_S: {
            if (editor->filename == NULL) {
                editor_startPrompt(editor, PROMPT_SAVENAME);
                return;
            }
            
            // Don't do anything if prompting something
            if (editor->isPrompting) return;
            
            editor_savefile(editor);

            break;
        }

        default: { 
            char *chrStr;
            
            if (e->key == TB_KEY_SPACE) chrStr = charAsString(' ');
            else chrStr = charAsString(e->ch);
            
            editor_appendToLine(editor->line, chrStr, editor->cursX);

            // Scroll on typing into scroll end
            if (editor->line->len - editor->scrollX >= renderer_getScreenWidth()) editor->scrollX++;
            
            editor->cursX++;
            
            free(chrStr);

            editor->isModified = 1;
            break;
        }

    }

}

void editor_freeEditor(struct Editor *editor) {

    for (struct EditorLine *line = editor->firstLine; line != NULL;) {
        struct EditorLine *n = line; // Store a temp pointer to move line pointer safely

        line = line->next;
        editor_freeLine(n);
    }

    free(editor);
}
