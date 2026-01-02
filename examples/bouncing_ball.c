#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../utils.c"
#include "../keyboard.c"

int main() {
    int fps = 25;
    screen.w = 128;
    screen.h = 36;
    object_t ball = {
        .size = {1, 1},
        .pos = {64, 18},
        .sprite = 'o'
    };
    
    point_t velocity = {2, 1};
    
    if (screen_arr != NULL) {
        free(screen_arr);
    }

    init_screen(' ');

    while (1) {
        put_screen_borders();
        put_object(ball);
        draw_screen();
        
        if (ball.pos.x + ball.size.w >= screen.w - 1 || ball.pos.x <= 1) {
            velocity.x *= -1;
        }

        if (ball.pos.y + ball.size.h >= screen.h - 1 || ball.pos.y <= 1) {
            velocity.y *= -1;
        }

        ball.pos = add_points(ball.pos, velocity);
        
        delay(1000000 / fps);
        clear_screen();
    }
    return 0;
}
