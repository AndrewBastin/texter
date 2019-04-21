#include <ncurses.h>

/* Initialize the curses display */
void curses_init() {
	// Ncurses initialization
	
	initscr();					// Start ncurses mode
	noecho();	 				// Disable echoing
	cbreak();					// Disable line buffering
	keypad(stdscr, true);		// Enable Special Characters
    nodelay(stdscr, true);      // Sets no delay
	start_color();				// Enable Color
}

/* De-initialize the curses display */
void curses_deinit() {
	endwin(); 					// Stops Ncurses mode
}

/* Gets the screen width */
int curses_getScreenWidth() {
	return getmaxx(stdscr);
}

/* Gets the screen height */
int curses_getScreenHeight() {
	return getmaxy(stdscr);
}

/* Gets a character */
int curses_getch() {
	return getch();
}

/* Draws a string at a given position */
void curses_drawText(int y, int x, char *str, int len) {
	if ((len >= x && len - x > curses_getScreenWidth()) || (len <= x && x - len > curses_getScreenWidth())) {
		mvaddnstr(y, x, str, curses_getScreenWidth() - 1);
	} else mvaddnstr(y, x, str, len);
}

void curses_drawChar(int y, int x, char ch) {
	mvaddch(y, x, ch);
}

/* Clears the curses screen */
void curses_clear() {
	erase();
}

/* Sets the rendered cursor position */
void curses_setCursorPos(int y, int x) {
	move(y, x);
}

/* Initializes a curses color pair */
void curses_createColorPair(short id, short fg, short bg) {
	init_pair(id, fg, bg);
}

/* Activates a color pair for rendering in ncurses */
void curses_activateColorPair(short id) {
	attron(COLOR_PAIR(id));
}

/* Deactivates a color pair for rendering in ncurses */
void curses_deactivateColorPair(short id) {
	attroff(COLOR_PAIR(id));
}

/* Draws a horizontal strip at the given line */
void curses_drawHorizStrip(int y) {
	mvhline(y, 0, 0, curses_getScreenWidth());
}

/* Turns on an ncurses attribute */
void curses_activateAttr(int attr) {
	attron(attr);
}

/* Turns off an ncurses attribute */
void curses_deactivateAttr(int attr) {
	attroff(attr);
}
