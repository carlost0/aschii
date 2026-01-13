#include <stdlib.h>
#include "../lib/utils.h"

int main() {
    scene_t main_scene = {
        .size = {64, 64},
        .screen = {}
    };

    rectangle_t rect = {
        .sprite = '#',
        .size = {20, 12},
        .pos = {24, 40}
    };

    init_scene(&main_scene);

    draw_rectangle(&main_scene, rect);
    print_scene(&main_scene);

    free(main_scene.screen);
    main_scene.screen = NULL;
    
    return 0;
}
