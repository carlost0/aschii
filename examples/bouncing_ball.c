#include <stdlib.h>
#include "../utils.c"
#include "../keyboard.c"

int main() {
    int fps = 25;

    screen.w = 128;
    screen.h = 36;

    object_t ball = {
        .size = {5, 3},
        .pos = {64, 18},
        .sprite = '@'
    };
    
    point_t ball_velocity = {2, 1};
    
    if (screen_arr != NULL) {
        free(screen_arr);
        screen_arr = NULL;
    }

    init_screen(' ');

    while (1) {
        put_screen_borders();
        put_object(ball);
        draw_screen();
        
        if (ball.pos.x + ball.size.w >= screen.w - 1 || ball.pos.x <= 1) {
            ball_velocity.x *= -1;
        }

        if (ball.pos.y + ball.size.h >= screen.h - 1 || ball.pos.y <= 1) {
            ball_velocity.y *= -1;
        }

        ball.pos = add_points(ball.pos, ball_velocity);
        
        delay(1000000 / fps);
        clear_screen();
    }
    return 0;
}
