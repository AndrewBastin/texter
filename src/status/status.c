#include "../editor/editor.h"
#include "../editor/line.h"
#include "../util/cursesutil.h"

void status_run(struct Editor *editor) {
    
    if (!editor->shouldRender) return;

    curses_activateAttr(CURSES_ATTR_REVERSE);

    // Printing spaces to fill the full bottom row
    for (int i = 0; i < curses_getScreenWidth(); i++) {
        curses_drawChar(curses_getScreenHeight() - 1, i, ' ');
    }

    curses_printf(
        curses_getScreenHeight() - 1, 
        0, 
        " %s %s [%d lines] %s", 
        (editor->filename == NULL) ? "" : editor->filename,
        (editor->doesFileExist) ? "" : "<New File>",
        editor->lineCount,
        (editor->isModified) ? "[+]" : ""
    );

    // Rendering position string on the right edge
    size_t sizeNeeded = snprintf(NULL, 0, "%d, %d", editor->cursY + 1, editor->cursX + 1) + 1; // Calculating the size of the string
    curses_printf(curses_getScreenHeight() - 1, curses_getScreenWidth() - sizeNeeded, "%d, %d", editor->cursY + 1, editor->cursX + 1);
    
    curses_deactivateAttr(CURSES_ATTR_REVERSE);

}
