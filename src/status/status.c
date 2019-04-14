#include "../editor/editor.h"
#include "../editor/line.h"
#include "../util/cursesutil.h"

void status_run(struct Editor *editor) {

    curses_activateAttr(CURSES_ATTR_REVERSE);

    curses_printf(
        curses_getScreenHeight() - 1, 
        0, 
        "%s\t\t[%d lines]\t %d, %d", 
        (editor->filename == NULL) ? "<New File>" : editor->filename, 
        editor->lineCount, 
        editor->cursY + 1, 
        editor->cursX + 1
    );
    
    curses_deactivateAttr(CURSES_ATTR_REVERSE);

}