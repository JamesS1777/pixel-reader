#ifndef PIXEL_H
#define PIXEL_H

typedef struct Color {
    int r;
    int g;
    int b;
} Color;

int colors_match(Color a, Color b, int tolerance);

#endif