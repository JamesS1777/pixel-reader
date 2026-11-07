#ifndef CAPTURE_H
#define CAPTURE_H

#include "pixel.h"
#include "window.h"

int capture_begin(const GameWindow *win);
Color capture_get_pixel(int x, int y);
void capture_end(void);

#endif