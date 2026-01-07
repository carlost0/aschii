#include "../types.h"
#include "../utils.c"
#include "../ascii.c"

int main() {
    screen.w = 474;
    screen.h = 474;

    ascii_object img = {
        .pos = {0, 0},
        .size = {474, 474},
        .sprite = {}
    };

    init_screen(' ');
    
    //replace path with path to your image
    img_to_ascii("/home/carlos/Pictures/arch_logo.bmp", &img);
    put_img(img);
    put_screen_borders();

    draw_screen();
    free(screen_arr);
    screen_arr = NULL;
}
