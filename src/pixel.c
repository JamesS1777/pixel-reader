#include <stdlib.h>
#include "pixel.h"

int colors_match(Color a, Color b, int tolerance) {
    if (a.r < 0 || b.r < 0) return 0;

    if (abs(a.r - b.r) > tolerance) return 0;
    if (abs(a.g - b.g) > tolerance) return 0;
    if (abs(a.b - b.b) > tolerance) return 0;
    return 1;
}