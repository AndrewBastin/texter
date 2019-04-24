#include <stdint.h>

#define RENDER_COLOR_BLACK      TB_BLACK
#define RENDER_COLOR_RED        TB_RED
#define RENDER_COLOR_GREEN      TB_GREEN
#define RENDER_COLOR_YELLOW     TB_YELLOW
#define RENDER_COLOR_BLUE       TB_BLUE
#define RENDER_COLOR_MAGENTA    TB_MAGENTA
#define RENDER_COLOR_CYAN       TB_CYAN
#define RENDER_COLOR_WHITE      TB_WHITE

// Macros
void renderer_init();
void renderer_deinit();
int renderer_getch();
void renderer_drawCharWithColor(int y, int x, char ch, uint16_t fg, uint16_t bg);
void renderer_drawChar(int y, int x, char ch);
void renderer_drawTextWithColor(int y, int x, char *str, int n, uint16_t fg, uint16_t bg);
void renderer_drawText(int y, int x, char *str, int n);
void renderer_clear();
void renderer_present();
void renderer_setCursorPos(int y, int x);
int renderer_getScreenWidth();
int renderer_getScreenHeight();
