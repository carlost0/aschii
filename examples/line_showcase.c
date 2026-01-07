#include "../utils.c" 
#include "../types.h"

int main() {
    screen.w = 32;
    screen.h = 16;

    line_t line = {
        .sprite = '#',
        .p1 = {0, 0},
        .p2 = {20, 16}
    };

    init_screen(' ');
    put_screen_borders();
    put_line(line);
    draw_screen();

    free(screen_arr);
    screen_arr = NULL;
    return 0;
}
