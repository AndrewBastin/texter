#include <string.h>

#include "../renderer/renderer.h"
#include "../util/stringutil.h"
#include "../editor/editor.h"
#include "../editor/line.h"

#define PROMPT_SAVENAME 1

/* Blanks the prompt bar */
void prompt_blank() {
    // Printing spaces to fill the full bottom row
    for (int i = 0; i < renderer_getScreenWidth(); i++) {
        renderer_drawChar(renderer_getScreenHeight() - 1, i, ' ');
    }
}

/* Sets the filename of the editor to the specific file */
void prompt_save(struct Editor *editor) {

    struct EditorLine *line = editor_createLine("", NULL, NULL);    // Create an editor instance to store and modify text
    int cursX = 0, scrollX = 0;

    short done = 0;
    int ch;

    while (!done) {

        // Draw string
        prompt_blank();
        renderer_drawText(renderer_getScreenHeight() - 1, 0, " Enter file name : ", 20);
        renderer_drawText(renderer_getScreenHeight() - 1, 19, line->str + scrollX, line->len - scrollX + 1);

        ch = renderer_getch();
        
        if (ch == CURSES_CH_ERR) continue;

        switch (ch) {
            
            case CURSES_KEY_LEFT:
                if (cursX > 0) {
                    if (cursX == scrollX) scrollX--;
                    cursX--;
                }
                break;

            case CURSES_KEY_RIGHT:
                if (cursX < line->len) {
                    if (cursX - scrollX == renderer_getScreenWidth() - 20) scrollX++;
                    cursX++;
                }
                break;

            case CURSES_KEY_BACKSPACE:
                if (cursX > 0) {
                    editor_deleteFromLine(line, cursX - 1, cursX - 1);
                    cursX--;

                }
                break;
            case '\n' :
                done = 1;
                break;

            default: {
                char *chrStr = charAsString(ch);
                editor_appendToLine(line, chrStr, cursX);

                cursX++;
                break;
            }
        }

    }

    editor->filename = (char *) malloc(sizeof(line->str));
    strcpy(editor->filename, line->str);

    editor_freeLine(line);                                          // Free line after use
}

void prompt_run(int promptType, struct Editor *editor) {
    renderer_activateAttr(CURSES_ATTR_REVERSE);
    
    prompt_blank();

    switch (promptType) {
        case PROMPT_SAVENAME:
            prompt_save(editor);
            break;
    }
    
    renderer_deactivateAttr(CURSES_ATTR_REVERSE);
}
