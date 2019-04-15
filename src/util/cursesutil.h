#include <ncurses.h>

#define CURSES_ATTR_REVERSE A_REVERSE

#define CURSES_CHAR_BLOCK ACS_BLOCK
#define CURSES_CHAR_CTRL(c) ((c) & 037)

#define CURSES_CH_ERR ERR
#define CURSES_KEY_ENTER KEY_ENTER
#define CURSES_KEY_BACKSPACE KEY_BACKSPACE
#define CURSES_KEY_UP KEY_UP
#define CURSES_KEY_DOWN KEY_DOWN
#define CURSES_KEY_LEFT KEY_LEFT
#define CURSES_KEY_RIGHT KEY_RIGHT

#define CURSES_COLOR_BLACK COLOR_BLACK
#define CURSES_COLOR_WHITE COLOR_WHITE


// Macros
#define curses_printf(y, x, str, ...) mvprintw(y, x, str, ##__VA_ARGS__)

void curses_init();
void curses_deinit();
int curses_getch();
void curses_drawText(int y, int x, char *str, int n);
void curses_drawChar(int y, int x, char ch);
void curses_clear();
void curses_setCursorPos(int y, int x);
void curses_createColorPair(short id, short fg, short bg);
void curses_activateColorPair(short id);
void curses_deactivateColorPair(short id);
int curses_getScreenWidth();
int curses_getScreenHeight();
void curses_drawHorizStrip(int y);
void curses_activateAttr(int attr);
void curses_deactivateAttr(int attr);