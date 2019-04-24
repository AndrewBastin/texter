#include <stdio.h>

#include "editor.h"
#include "line.h"
#include "renderer.h"


void status_fill() {
    for (int i = 0; i < renderer_getScreenWidth(); i++) {
        renderer_drawCharWithColor(renderer_getScreenHeight() - 1, i, ' ', RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);
    }
}

void status_run(struct Editor *editor) {
    
    if (!editor->shouldRender) return;
    
    status_fill();

    size_t leftReqSize = snprintf(
        NULL,
        0,
        " %s %s [%d lines] %s",
        (editor->filename == NULL) ? "" : editor->filename,
        (editor->doesFileExist) ? "" : "<New File>",
        editor->lineCount,
        (editor->isModified) ? "[+]" : ""
    );

    char *leftStr = malloc(leftReqSize * sizeof(char));

    snprintf(
        leftStr,
        leftReqSize,
        " %s %s [%d lines] %s ",
        (editor->filename == NULL) ? "" : editor->filename,
        (editor->doesFileExist) ? "" : "<New File>",
        editor->lineCount,
        (editor->isModified) ? "[+]" : ""
    );

    renderer_drawTextWithColor(renderer_getScreenHeight() - 1, 0, leftStr, leftReqSize, RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);
    
    free(leftStr);


    size_t rightReqSize = snprintf(
        NULL, 
        0,
        "%d,%d ",
        editor->cursY + 1,
        editor->cursX + 1
    );

    char *rightStr = malloc(rightReqSize * sizeof(char));
    
    snprintf(
        rightStr,
        rightReqSize,
        "%d,%d ",
        editor->cursY + 1,
        editor->cursX + 1
    );

    renderer_drawTextWithColor(renderer_getScreenHeight() - 1, renderer_getScreenWidth() - rightReqSize, rightStr, rightReqSize, RENDER_COLOR_BLACK, RENDER_COLOR_WHITE);

    free(rightStr);
}
