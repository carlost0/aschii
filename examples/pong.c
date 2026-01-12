#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include "../utils.h"
#include "../keyboard.h"

typedef enum {
    ONE_P,
    TWO_P,
    QUIT
} selection;


selection start_screen() {
    int fps = 10;
    selection state = 0;

    scene_t select_scene = {
        .size = {128, 36}
    };

    text_t title = {
        .str = "PONG",
        .pos = {62, 10} //20
    };

    text_t one_player = {
        .str = "one player mode",
        .pos = {57, 14} // 24
    };
    
    text_t two_player = {
        .str = "two player mode",
        .pos = {57, 17} // 27
    };

    text_t quit = {
        .str = "quit",
        .pos = {62, 20} // 30
    };

    rectangle_t background = {
        .pos = {33, 12}, // 22
        .size = {64, 11}, // 11
        .sprite = ' '
    };

    rectangle_t cursor = {
        .pos = {55, 13}, // 23
        .size = {20, 3}, // 3
        .sprite = '-'
    };
    
    line_t outline_1 = {
        .p1 = background.pos,
        .p2 = {background.pos.x + background.size.w, background.pos.y},
        .sprite = '#'
    };

    line_t outline_2 = {
        .p1 = background.pos,
        .p2 = {background.pos.x, background.pos.y + background.size.h},
        .sprite = '#'
    };

    line_t outline_3 = {
        .p1 = {background.pos.x + background.size.w, background.pos.y + background.size.h},
        .p2 = {background.pos.x + background.size.w, background.pos.y},
        .sprite = '#'
    };

    line_t outline_4 = {
        .p1 = {background.pos.x + background.size.w, background.pos.y + background.size.h},
        .p2 = {background.pos.x, background.pos.y + background.size.h},
        .sprite = '#'
    };
    init_scene(&select_scene, ' ');
    INIT_INPUT

    while (1) {
        put_screen_borders(&select_scene);
        put_rectangle(&select_scene, background);
        put_rectangle(&select_scene, cursor);
        put_text_horizontal(&select_scene, quit);
        put_text_horizontal(&select_scene, one_player);
        put_text_horizontal(&select_scene, two_player);
        put_text_horizontal(&select_scene, title);
        put_line(&select_scene, outline_1);
        put_line(&select_scene, outline_2);
        put_line(&select_scene, outline_3);
        put_line(&select_scene, outline_4);
        draw_scene(&select_scene);

        GET_INPUT

        if (input == 'w' && state > 0) {
            state--;
            cursor.pos.y -= 3;
        } else if (input == 's' && state < 2) {
            state++;
            cursor.pos.y += 3;
        }
        
        else if (input == ' ') {
            END_INPUT
            free(select_scene.screen);
            return state;
        }

        clear_scene(&select_scene);
        delay(1000000 / fps);
    }
}

void one_player_mode() {
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

void two_player_mode() {
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
                break;
            case 's':
                if (paddle_1.pos.y + paddle_1.size.h + 1 < scene.size.h && turn < 0) paddle_1.pos.y++;
                break;
            case 'i':
                if (turn > 0 && paddle_2.pos.y - 1 > 0) paddle_2.pos.y--;
                break;
            case 'k':
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

int main() {
    selection sel = start_screen();
    switch (sel) {
        case ONE_P:
            one_player_mode();
            break;
        case TWO_P:
            two_player_mode();
            break;
        case QUIT:
            break;
    }
    
}
