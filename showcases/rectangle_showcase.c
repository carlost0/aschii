#include <stdlib.h>
#include "../lib/utils.h"

int main() {
    color_t color = {255, 0, 0};

    scene_t main_scene = {
        .size = {64, 64},
        .screen = {}
    };

    rectangle_t rect = {
        .sprite = '#',
        .size = {20, 12},
        .pos = {24, 40},
        .color = {0, 255, 0}
    };

    init_scene(&main_scene);

    draw_screen_borders(&main_scene, color);
    draw_rectangle(&main_scene, rect);
    print_scene(&main_scene);

    free(main_scene.screen);
    main_scene.screen = NULL;
    
    return 0;
}
