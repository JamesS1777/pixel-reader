#include <stdio.h>

struct Color {
    int r;
    int g;
    int b;
};

int main(void) {
    struct Color sky = {135, 206, 235};
    struct Color grass = {34, 139, 34};


    printf("sky: R=%d G=%d B=%d\n", sky.r, sky.g, sky.b);
    printf("grass: R=%d G=%d B=%d\n", grass.r, grass.g, grass.b);

    sky.b = 200;
    printf("sky after change: R=%d G=%d B=%d\n", sky.r, sky.g, sky.b);

    return 0;
}