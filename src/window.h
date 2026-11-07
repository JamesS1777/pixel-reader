#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

/* The game area (no title bar or borders), in screen pixels */
typedef struct {
    HWND hwnd;
    int left;
    int top;
    int width;
    int height;
} GameWindow;

int window_find(const char *title_part, GameWindow *win);
int window_update(GameWindow *win);
void window_focus(const GameWindow *win);
void window_list_titles(void);

#endif