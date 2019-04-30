#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <termbox.h>

#include "renderer.h"
#include "editor.h"
#include "status.h"


struct Editor *editor = NULL;

int main(int argc, char *argv[]) {

	if (argc > 2) {
		printf("ERROR : Too many arguments supplied \n\n");
		printf("Usage : \n");
		printf("texter (file to load (optional))\n");

		return -1;
	} else if (argc == 1) {
		editor = editor_createBlankEditor();
	} else if (argc == 2) {
		editor = editor_createEditorFromFile(argv[1]);

		if (editor == NULL) {
			printf("ERROR : Failed loading the file...\n");

			return -2;
		}
	}

	renderer_init();

    struct tb_event *ev = malloc(sizeof(struct tb_event));

	while (1) {
        
        renderer_clear();

		status_render(editor);

		editor_render(editor);

        tb_present();

        tb_poll_event(ev);
	    
        if (ev->key == TB_KEY_CTRL_C) break;
        
        editor_input(editor, ev);
    }
    
    free(ev);
    renderer_deinit();
    editor_freeEditor(editor);


	return 0;
}
