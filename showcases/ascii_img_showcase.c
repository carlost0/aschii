#include "../lib/utils.h"
#include <stdlib.h>
//#include "../ascii.c"

int main() {
    scene_t main_scene = {
        .screen = {},
        .size = {200, 242}
    };

    img_object_t img = {
        .pos = {0, 0},
        .size = {200, 242},
        .sprite = {}
    };

    init_scene(&main_scene);

    //replace path with path to your image
    img_to_ascii("/home/carlos/Pictures/thinkpad.bmp", &img);
    draw_img(&main_scene, img);

    print_scene(&main_scene);
    free(main_scene.screen);
    main_scene.screen = NULL;
}
