#include <stdio.h>
#include "../utils.c"
#include "../keyboard.c"

int main() {
    int fps = 25;
    char c = 0;
    int score[2] = {48, 48}; 
    int reset = 0;

    screen.w = 128;
    screen.h = 36;

    object_t ball = {
        .sprite = '@',
        .size = {2, 2},
        .pos = {64, 18}
    };

    point_t ball_velocity = {1, 1};
    
    object_t paddle_1 = {
        .sprite = '#',
        .size = {1, 8},
        .pos = {2, 12}
    };

    object_t paddle_2 = {
        .sprite = '#',
        .size = {1, 8},
        .pos = {125, 12}
    };
    
    object_t score_1 = {
        .sprite = (char) score[0],
        .size = {1, 1},
        .pos = {4, 8}
    };

    object_t score_2 = {
        .sprite = (char) score[1],
        .size = {1, 1},
        .pos = {123, 8}
    };

    init_screen(' ');
    disable_canonical_input();
    while (1) {
        put_screen_borders();
        put_object(paddle_1);
        put_object(paddle_2);
        put_object(ball);
        put_object(score_1);
        put_object(score_2);
    
        score_1.sprite = (char) score[0];
        score_2.sprite = (char) score[1];

        c = get_keyboard_input();

        if (c == 'q') {
            break;
        }

        if (c == 'w' && paddle_1.pos.y - 1 > 0) {
            paddle_1.pos.y--;
        } else if (c == 's' && paddle_1.pos.y + paddle_1.size.h + 1 < screen.h) {
            paddle_1.pos.y++;
        }
        
        if (c == 'i' && paddle_2.pos.y -  1 > 0) {
            paddle_2.pos.y--;
        } else if (c == 'k' && paddle_2.pos.y + paddle_2.size.h + 1 < screen.h) {
            paddle_2.pos.y++;
        }

        if (ball.pos.x + ball.size.w >= screen.w - 1) {
            reset = 1;
            score[0] += 1;
        } else if (ball.pos.x <= 1) {
            reset = 1;
            score[1] += 1;
        }
        
        if (reset == 1) {
            ball.pos.x = 62;
            ball.pos.y = 16;
            paddle_1.pos.y = 12;
            paddle_2.pos.y = 12;
            ball_velocity.x = 1;
            ball_velocity.y = 1;
            reset = 0;
        }
        
        if (check_collision(paddle_1, ball) == 1 || check_collision(paddle_2, ball) == 1) {
            ball_velocity.x *= -1;
        }

        if (ball.pos.y + ball.size.h >= screen.h - 1 || ball.pos.y <= 1) {
            ball_velocity.y *= -1;
        }
        
        if (score[0] == 53) {
            clear_screen();
            printf("Player 1 won!");
            delay(1000);
            break;
        } else if (score[1] == 53) {
            clear_screen();
            printf("Player 2 won!");
            delay(1000);
            break;
        }
        //add velocity vector to position
        ball.pos = add_points(ball.pos, ball_velocity);
        draw_screen();
        clear_screen();
        delay(1000000 / fps);
    }

    enable_canonical_input();
    free(screen_arr);
    screen_arr = NULL;
}
