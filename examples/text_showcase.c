#include <stdlib.h>
#include "../utils.c"
#include "../keyboard.c"

int main() {
    screen.w = 32;
    screen.h = 16;

    text_t text_horizontal = {
        .pos = {10, 0},
        .str = "i'm horizonal!"
    };

    text_t text_vertical = {
        .pos = {6, 2},
        .str = "i'm vertical!"
    };

    init_screen(' ');
    put_text_horizontal(text_horizontal);
    put_text_vertical(text_vertical);
    draw_screen();
    free(screen_arr);
    screen_arr = NULL;
    return 0;
}

