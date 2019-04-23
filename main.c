#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <termbox.h>

#include "renderer.h"
#include "editor.h"
#include "status.h"


struct Editor *editor = NULL;


void signalHandler(int sig) {
	if (sig == SIGINT) {
		renderer_deinit();

		if (editor != NULL) editor_freeEditor(editor);
		exit(0);
	}
}


int main(int argc, char *argv[]) {

	signal(SIGINT, signalHandler);						// Set Signal Handler

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

		status_run(editor);

		editor_render(editor);

        tb_peek_event(ev, 10);
	}

	return 0;
}
