#include <string.h>
#include <termbox.h>

#include "renderer.h"
#include "stringutil.h"
#include "editor.h"
#include "line.h"

#include "prompt_save.h"

#define PROMPT_SAVENAME 1

/* Blanks the prompt bar */
void prompt_blank() {
    // Printing spaces to fill the full bottom row
    for (int i = 0; i < renderer_getScreenWidth(); i++) {
        renderer_drawCharWithColor(renderer_getScreenHeight() - 1, i, ' ', RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);
    }
}

void prompt_init(struct Editor *editor) {
  switch (editor->promptType) {
    case PROMPT_SAVENAME:
      prompt_save_init(editor);
      break;
  }
}

/* Sets the filename of the editor to the specific file */
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
