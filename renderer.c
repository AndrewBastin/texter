#include <termbox.h>

/* Initialize the curses display */
void renderer_init() {
    tb_init();	
}

/* De-initialize the curses display */
void renderer_deinit() {
    tb_shutdown();
}

/* Gets the screen width */
int renderer_getScreenWidth() {
	return tb_width();
}

/* Gets the screen height */
int renderer_getScreenHeight() {
	return tb_height();
}

void renderer_drawCharWithColor(int y, int x, char ch, uint16_t fg, uint16_t bg) {
    tb_change_cell(x, y, ch, fg, bg);
}

void renderer_drawChar(int y, int x, char ch) {
    tb_change_cell(x, y, ch, TB_WHITE, TB_BLACK);
}

/* Draws a string at a given position */
void renderer_drawText(int y, int x, char *str, int len) {
   for (int pos = 0; (pos < len) && (x + pos < renderer_getScreenWidth()); pos++) {
       renderer_drawChar(y, x + pos, str[pos]);
   }
}

/* Draw text with color */
void renderer_drawTextWithColor(int y, int x, char *str, int len, uint16_t fg, uint16_t bg) {
    for (int pos = 0; (pos < len) && (x + pos < renderer_getScreenWidth()); pos++) {
        renderer_drawCharWithColor(y, x + pos, str[pos], fg, bg); 
    }
}

/* Clears the curses screen */
void renderer_clear() {
    tb_clear();
}

/* Draws the backbuffer on screen */
void renderer_present() {
    tb_present();
}

/* Sets the rendered cursor position */
void renderer_setCursorPos(int y, int x) {
    tb_set_cursor(x, y);
}
