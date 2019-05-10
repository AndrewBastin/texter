#include <termbox.h>

#include "editor.h"

struct prompt_open_state {
  
  int cursX;
  char *filename;

};

void prompt_open_init(struct Editor *editor);
void prompt_open_render(struct Editor *editor);
void prompt_open_input(struct Editor *editor, struct tb_event *ev);
