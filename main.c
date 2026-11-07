#include <stdio.h>

struct Color {
    int r;
    int g;
    int b;
};

int main(void) {
    struct Color sky = {135, 206, 235};
    struct Color grass = {34, 139, 34};
    struct Color apple = {255, 0, 0};


    printf("sky: R=%d G=%d B=%d\n", sky.r, sky.g, sky.b);
    printf("grass: R=%d G=%d B=%d\n", grass.r, grass.g, grass.b);
    printf("apple: R=%d G=%d B=%d\n", apple.r, apple.g, apple.b);

    sky.b = 200;
    printf("sky after change: R=%d G=%d B=%d\n", sky.r, sky.g, sky.b);

    return 0;
}