#include <stdlib.h>
#include "../utils.c"
#include "../keyboard.c"

int main() {
    screen.w = 32;
    screen.h = 16;

    text_t text = {
        .pos = {10, 8},
        .str = "hello, world!"
    };

    init_screen(' ');
    put_screen_borders();
    put_text(text);
    draw_screen();
    free(screen_arr);
    screen_arr = NULL;
    return 0;
}

