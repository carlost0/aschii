#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include "../utils.h"
#include "../keyboard.h"

int main() {
    INIT_INPUT
    int fps = 25;
    int score[2] = {48, 48}; 
    int reset = 0;
    int turn;

    srand(time(0));

    scene_t scene = {
        .size = {128, 36},
    };

    rectangle_t ball = {
        .sprite = '@',
        .size = {2, 2},
        .pos = {64, 18}
    };

    text_t instruction = {
        .str = "press w/s to move",
        .pos = {0, 0}
    };

    point_t ball_velocity = {((rand() % 2) == 1 ? 1 : -1), ((rand() % 2) == 1 ? 1 : -1)};
    
    rectangle_t paddle_1 = {
        .sprite = '#',
        .size = {1, 8},
        .pos = {2, 12}
    };

    rectangle_t paddle_2 = {
        .sprite = '#',
        .size = {1, 8},
        .pos = {125, 12}
    };
    
    rectangle_t score_1 = {
        .sprite = (char) score[0],
        .size = {1, 1},
        .pos = {4, 8}
    };

    rectangle_t score_2 = {
        .sprite = (char) score[1],
        .size = {1, 1},
        .pos = {123, 8}
    };


    init_scene(&scene, ' ');
    while (input != 'q') {
        GET_INPUT

        put_screen_borders(&scene);
        put_rectangle(&scene, paddle_1);
        put_rectangle(&scene, paddle_2);
        put_rectangle(&scene, ball);
        put_rectangle(&scene, score_1);
        put_rectangle(&scene, score_2);
        put_text_horizontal(&scene, instruction);
    
        score_1.sprite = (char) score[0];
        score_2.sprite = (char) score[1];

        turn = ball_velocity.x;

        switch (input) {
            case 'w':
                if (turn < 0 && paddle_1.pos.y - 1 > 0) paddle_1.pos.y--;
                if (turn > 0 && paddle_2.pos.y - 1 > 0) paddle_2.pos.y--;
                break;
            case 's':
                if (paddle_1.pos.y + paddle_1.size.h + 1 < scene.size.h && turn < 0) paddle_1.pos.y++;
                if (paddle_2.pos.y + paddle_2.size.h + 1 < scene.size.h && turn > 0) paddle_2.pos.y++;
                break;
            default:
                break;
        }

        if (ball.pos.x + ball.size.w >= scene.size.w - 1) {
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
            ball_velocity.x = rand() % 2 == 1 ? 1 : -1;
            ball_velocity.y = rand() % 2 == 1 ? 1 : -1;
            reset = 0;
        }
        
        if (check_collision(paddle_1, ball)) {
            ball_velocity.x *= -1;
        }

        if (check_collision(paddle_2, ball)) {
            ball_velocity.x *= -1;
        }

        if (ball.pos.y + ball.size.h >= scene.size.h - 1 || ball.pos.y <= 1) {
            ball_velocity.y *= -1;
        }
        
        if (score[0] == 53) {
            clear_scene(&scene);
            printf("Player 1 won!");
            delay(100000);
            break;
        } else if (score[1] == 53) {
            clear_scene(&scene);
            printf("Player 2 won!");
            delay(100000);
            break;
        }

        //add velocity vector to position
        ball.pos = add_points(ball.pos, ball_velocity);
        draw_scene(&scene);
        clear_scene(&scene);
        delay(1000000 / fps);
    }


    END_INPUT
    free(scene.screen);
    scene.screen = NULL;
}
