#include <stdlib.h>
#include "../lib/utils.h"

int main() {
    scene_t main_scene = {
        .size = {64, 64},
        .screen = {},
    };

    line_t line = {
        .sprite = '#',
        .p1 = {0, 0},
        .p2 = {20, 16}
    };

    line_t line2 = {
        .sprite = 'o',
        .p1 = {0, 16},
        .p2 = {32, 0}
    };

    init_scene(&main_scene);
    draw_screen_borders(&main_scene);
    draw_line(&main_scene, line);
    draw_line(&main_scene, line2);
    print_scene(&main_scene);

    free(main_scene.screen);
    main_scene.screen = NULL;
    return 0;
}
