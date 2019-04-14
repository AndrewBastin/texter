#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "util/cursesutil.h"
#include "editor/editor.h"
#include "status/status.h"


struct Editor *editor = NULL;


void signalHandler(int sig) {
	if (sig == SIGINT) {
		curses_deinit();

		if (editor != NULL) editor_freeEditor(editor);
		exit(0);
	}
}


int main() {

	signal(SIGINT, signalHandler);						// Set Signal Handler

	curses_init();

	editor = editor_createBlankEditor();

	while (1) {

		status_run(editor);
		
		editor_run(editor);

	}

	while (curses_getch());
	
	return 0;
}
