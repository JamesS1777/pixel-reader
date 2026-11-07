/* Shows your cursor position inside the game and the colour under it.
   F8 prints a line for config.ini. Ctrl+C to quit. */

#include <windows.h>
#include <stdio.h>
#include "config.h"
#include "window.h"
#include "capture.h"

int main(void) {
    SetProcessDPIAware();

    Config cfg;
    if (!config_load("config.ini", &cfg)) {
        printf("Couldn't open config.ini. Run from the project folder.\n");
        return 1;
    }

    GameWindow win;
    if (!window_find(cfg.window_title, &win)) {
        printf("No window title contains \"%s\".\n", cfg.window_title);
        printf("Copy part of the right title into window_title in config.ini.\n\n");
        window_list_titles();
        return 1;
    }

    printf("Game area: %d x %d\n", win.width, win.height);
    printf("Hover over something and press F8. Lines are saved to calibrate.txt.\n");
    printf("Ctrl+C to quit.\n\n");

    int f8_was_down = 0;
    while (1) {
        if (!window_update(&win)) { printf("\nGame window closed.\n"); break; }

        POINT cursor;
        GetCursorPos(&cursor);
        int x = cursor.x - win.left;
        int y = cursor.y - win.top;
        int inside = x >= 0 && y >= 0 && x < win.width && y < win.height;

        capture_begin(&win);
        Color c = capture_get_pixel(x, y);
        capture_end();

        if (inside) printf("\r  x=%5d  y=%5d   R=%3d G=%3d B=%3d          ", x, y, c.r, c.g, c.b);
        else        printf("\r  (cursor is outside the game area)              ");
        fflush(stdout);

        int f8_down = (GetAsyncKeyState(VK_F8) & 0x8000) != 0;
        if (f8_down && !f8_was_down) {
            if (!inside) {
                printf("\n  Move the cursor into the game first.\n");
            } else {
                printf("\n  button or slot:  %d,%d\n", x, y);
                printf("  state check:     %d,%d,%d,%d,%d\n", x, y, c.r, c.g, c.b);
                FILE *out = fopen("calibrate.txt", "a");
                if (out != NULL) {
                    fprintf(out, "point %d,%d    check %d,%d,%d,%d,%d\n", x, y, x, y, c.r, c.g, c.b);
                    fclose(out);
                }
            }
        }
        f8_was_down = f8_down;

        Sleep(100);
    }
    return 0;
}