#include <stdlib.h>
#include "../utils.h"
#include "../keyboard.c"

int main() {
    int fps = 25;

    scene_t scene = {
        .size = {128, 36},
        .screen = {}
    };

    rectangle_t ball = {
        .size = {5, 3},
        .pos = {64, 18},
        .sprite = '@'
    };
    
    point_t ball_velocity = {2, 1};
    
    if (scene.screen != NULL) {
        free(scene.screen);
        scene.screen = NULL;
    }

    init_scene(&scene, ' ');

    while (1) {
        put_screen_borders(&scene);
        put_rectangle(&scene, ball);
        draw_scene(&scene);
        
        if (ball.pos.x + ball.size.w >= scene.size.w - 1 || ball.pos.x <= 1) {
            ball_velocity.x *= -1;
        }

        if (ball.pos.y + ball.size.h >= scene.size.h - 1 || ball.pos.y <= 1) {
            ball_velocity.y *= -1;
        }

        ball.pos = add_points(ball.pos, ball_velocity);
        
        delay(1000000 / fps);
        clear_scene(&scene);
    }
    return 0;
}
