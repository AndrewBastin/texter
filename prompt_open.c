#include <string.h>
#include <stdlib.h>
#include <termbox.h>

#include "editor.h"
#include "renderer.h"
#include "prompt.h"
#include "stringutil.h"
#include "prompt_open.h"

void prompt_input_open_backspace(struct Editor *editor) {
  
  struct prompt_open_state *state = editor->promptState;
  
  if (state->cursX > 0) {
        char *newStr = deleteAtIndex(state->filename, state->cursX - 1, state->cursX - 1);

        free(state->filename);
        state->filename = newStr;

        state->cursX--;
    }
}


void prompt_open_init(struct Editor *editor) {
  
  struct prompt_open_state *state = malloc(sizeof(struct prompt_open_state));
  state->filename = charAsString('\0');
  state->cursX = 0;
  
  editor->promptState = state;
}


void prompt_open_render(struct Editor *editor) {
  
  struct prompt_open_state *state = editor->promptState;

  renderer_drawTextWithColor(renderer_getScreenHeight() - 1, 0, "Enter file name : ", 20, RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);
  renderer_drawTextWithColor(renderer_getScreenHeight() - 1, 19, state->filename, strlen(state->filename), RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);
  renderer_setCursorPos(renderer_getScreenHeight() - 1, 19 + state->cursX);

}

void prompt_open_input(struct Editor *editor, struct tb_event *ev) {
    
  struct prompt_open_state *state = editor->promptState;

    switch (ev->key) {
        
        case TB_KEY_ESC:
          editor_closePrompt(editor);
          break;


        case TB_KEY_ENTER: {
          char *filename = malloc(sizeof(state->filename));
          strcpy(filename, state->filename);
          editor_freeEditor(editor);
          
          editor = editor_createEditorFromFile(filename);
          break;
        }

        case TB_KEY_BACKSPACE2:
            prompt_input_open_backspace(editor);
            break;

        case TB_KEY_BACKSPACE:
            prompt_input_open_backspace(editor);
            break;

        case TB_KEY_ARROW_LEFT:
            if (state->cursX > 0) {
                state->cursX--;
            }
            break;

        case TB_KEY_ARROW_RIGHT:
            if (state->cursX < strlen(state->filename)) {
                state->cursX++;
            }
            break;

        default: {
            char *chrStr;

            if (ev->key == TB_KEY_SPACE) chrStr = charAsString(' ');
            else chrStr = charAsString(ev->ch);
            
            char *str = appendAtIndex(state->filename, chrStr, state->cursX);

            state->cursX++;

            free(state->filename);
            state->filename = str;

            free(chrStr);

            break;
        }

    }

}

