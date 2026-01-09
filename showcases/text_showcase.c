#include <stdlib.h>
#include "../utils.h"

int main() {
    scene_t main_scene = {
        .size = {32, 32},
        .screen = {}
    };

    text_t text_horizontal = {
        .pos = {10, 0},
        .str = "i'm horizonal!"
    };

    text_t text_vertical = {
        .pos = {6, 2},
        .str = "i'm vertical!"
    };

    init_scene(&main_scene, ' ');
    put_text_horizontal(&main_scene, text_horizontal);
    put_text_vertical(&main_scene, text_vertical);
    draw_scene(&main_scene);
    free(main_scene.screen);
    main_scene.screen = NULL;
    return 0;
}

