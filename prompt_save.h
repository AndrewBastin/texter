#include <termbox.h>

#include "editor.h"

struct prompt_save_state {
  int cursX;
  char *filename;
};

void prompt_save_init(struct Editor *editor);
void prompt_save_render(struct Editor *editor);
void prompt_save_input(struct Editor *editor, struct tb_event *ev);
