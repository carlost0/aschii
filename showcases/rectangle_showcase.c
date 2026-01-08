#include "../types.h"
#include "../utils.c"

int main() {
    screen.w = 64;
    screen.h = 64;

    rectangle_t rect = {
        .sprite = '#',
        .size = {20, 12},
        .pos = {24, 40}
    };

    init_screen(' ');

    put_rectangle(rect);
    draw_screen();

    free(screen_arr);
    screen_arr = NULL;
    
    return 0;
}
