#include "editor.h"

#define PROMPT_SAVENAME 1

void prompt_init(struct Editor *editor);
void prompt_render(struct Editor *editor);
void prompt_input(struct Editor *editor, struct tb_event *ev);
void prompt_input_backspace(struct Editor *editor);
