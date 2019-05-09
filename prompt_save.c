#include <string.h>
#include <stdlib.h>
#include <termbox.h>

#include "editor.h"
#include "renderer.h"
#include "prompt.h"
#include "stringutil.h"

void prompt_save_render(struct Editor *editor) {
  
  renderer_drawTextWithColor(renderer_getScreenHeight() - 1, 0, "Enter file name : ", 20, RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);
  renderer_drawTextWithColor(renderer_getScreenHeight() - 1, 19, editor->promptLine, strlen(editor->promptLine), RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);
  renderer_setCursorPos(renderer_getScreenHeight() - 1, 19 + editor->promptCursX);

}

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

