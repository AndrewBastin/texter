#include <string.h>
#include <termbox.h>

#include "renderer.h"
#include "stringutil.h"
#include "editor.h"
#include "line.h"

#define PROMPT_SAVENAME 1

/* Blanks the prompt bar */
void prompt_blank() {
    // Printing spaces to fill the full bottom row
    for (int i = 0; i < renderer_getScreenWidth(); i++) {
        renderer_drawCharWithColor(renderer_getScreenHeight() - 1, i, ' ', RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);
    }
}

void prompt_save_render(struct Editor *editor) {
    
    renderer_drawTextWithColor(renderer_getScreenHeight() - 1, 0, " Enter file name : ", 20, RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);
    renderer_drawTextWithColor(renderer_getScreenHeight() - 1, 19, editor->promptLine, strlen(editor->promptLine), RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);
    
    renderer_setCursorPos(renderer_getScreenHeight() - 1, 19 + editor->promptCursX);
}

void prompt_input_backspace(struct Editor *editor) {
    if (editor->promptCursX > 0) {
        char *newStr = deleteAtIndex(editor->promptLine, editor->promptCursX - 1, editor->promptCursX - 1);

        free(editor->promptLine);
        editor->promptLine = newStr;

        editor->promptCursX--;
    }
}

/* Sets the filename of the editor to the specific file */
void prompt_save_input(struct Editor *editor, struct tb_event *ev) {
    
    switch (ev->key) {
        
        case TB_KEY_ESC:
            editor_closePrompt(editor);
            break;


        case TB_KEY_ENTER:
            editor->filename = editor->promptLine;
            editor_savefile(editor);
            editor_closePrompt(editor);
            break;

        case TB_KEY_BACKSPACE2:
            prompt_input_backspace(editor);
            break;

        case TB_KEY_BACKSPACE:
            prompt_input_backspace(editor);
            break;

        case TB_KEY_ARROW_LEFT:
            if (editor->promptCursX > 0) {
                editor->promptCursX--;
            }
            break;

        case TB_KEY_ARROW_RIGHT:
            if (editor->promptCursX < strlen(editor->promptLine)) {
                editor->promptCursX++;
            }
            break;

        default: {
            char *chrStr;

            if (ev->key == TB_KEY_SPACE) chrStr = charAsString(' ');
            else chrStr = charAsString(ev->ch);
            
            char *str = appendAtIndex(editor->promptLine, chrStr, editor->promptCursX);

            editor->promptCursX++;

            free(editor->promptLine);
            editor->promptLine = str;

            free(chrStr);

            break;
        }

    }

}

void prompt_render(struct Editor *editor) {
    prompt_blank();

    switch (editor->promptType) {
        case PROMPT_SAVENAME:
            prompt_save_render(editor);
            break;
    }
}

void prompt_input(struct Editor *editor, struct tb_event *ev) {
    
    switch (editor->promptType) {
        case PROMPT_SAVENAME:
            prompt_save_input(editor, ev);
            break;
    }
    
}
