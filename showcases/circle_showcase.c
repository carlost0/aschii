#include <stdlib.h>
#include "../utils.h"

int main() {
    scene_t main_scene = {
        .size = {128, 128},
        .screen = {}
    };

    circle_t circle = {
        .sprite = '#',
        .radius = 24,
        .pos = {64, 64}
    };

    init_scene(&main_scene, ' ');

    put_circle(&main_scene, circle);
    
    draw_scene(&main_scene);
    free(main_scene.screen);
    main_scene.screen = NULL;

    return 0;
}
