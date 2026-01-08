#include "../types.h"
#include "../utils.c"

int main() {
    screen.w = 128;
    screen.h = 128;

    circle_t circle = {
        .sprite = '#',
        .radius = 24,
        .pos = {64, 64}
    };

    init_screen(' ');

    put_circle(circle);
    
    draw_screen();
    free(screen_arr);
    screen_arr = NULL;

    return 0;
}
