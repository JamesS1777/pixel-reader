#include "capture.h"

/* SLOW VERSION: one GetPixel call per pixel. Fine for a few checks.
   Reads from the whole screen because Google Play Games draws with the
   graphics card, and reading its window directly can return black.
   TODO (Stage 5): fast version with BitBlt + GetDIBits. */

static const GameWindow *g_win = NULL;
static HDC g_screen = NULL;

int capture_begin(const GameWindow *win) {
    g_win = win;
    g_screen = GetDC(NULL);
    return g_screen != NULL;
}

Color capture_get_pixel(int x, int y) {
    Color c = { -1, -1, -1 };
    if (g_screen == NULL || g_win == NULL) return c;
    if (x < 0 || y < 0 || x >= g_win->width || y >= g_win->height) return c;

    COLORREF ref = GetPixel(g_screen, g_win->left + x, g_win->top + y);
    if (ref == CLR_INVALID) return c;

    c.r = GetRValue(ref);
    c.g = GetGValue(ref);
    c.b = GetBValue(ref);
    return c;
}

void capture_end(void) {
    if (g_screen != NULL) {
        ReleaseDC(NULL, g_screen);
        g_screen = NULL;
    }
}