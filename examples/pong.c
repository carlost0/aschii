#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include "../lib/utils.h"
#include "../lib/keyboard.h"

typedef enum {
    ONE_P,
    TWO_P,
    QUIT
} selection;

error_e err;

selection start_screen() {
    int fps = 10;
    selection state = 0;

    scene_t select_scene = {
        .size = {128, 36}
    };

    text_t title = {
        .str = "PONG",
        .pos = {62, 10}, //20
        .color = {0, 255, 0}
    };

    text_t one_player = {
        .str = "one player mode",
        .pos = {57, 14}, // 24
        .color = {255, 255, 255}
    };
    
    text_t two_player = {
        .str = "two player mode",
        .pos = {57, 17}, // 27
        .color = {255, 255, 255}
    };

    text_t quit = {
        .str = "quit",
        .pos = {62, 20}, // 30
        .color = {255, 0, 0}
    };

    rectangle_t background = {
        .pos = {33, 12}, // 22
        .size = {64, 11}, // 11
        .sprite = '#',
        .color = {0, 0, 0}
    };

    rectangle_t cursor = {
        .pos = {55, 13}, // 23
        .size = {20, 3}, // 3
        .sprite = '-',
        .color = {100, 100, 100}
    };
    
    line_t outline_1 = {
        .p1 = background.pos,
        .p2 = {background.pos.x + background.size.w, background.pos.y},
        .sprite = '#',
        .color = {255, 255, 255},
    };

    line_t outline_2 = {
        .p1 = background.pos,
        .p2 = {background.pos.x, background.pos.y + background.size.h},
        .sprite = '#',
        .color = {255, 255, 255},
    };

    line_t outline_3 = {
        .p1 = {background.pos.x + background.size.w, background.pos.y + background.size.h},
        .p2 = {background.pos.x + background.size.w, background.pos.y},
        .sprite = '#',
        .color = {255, 255, 255},
    };

    line_t outline_4 = {
        .p1 = {background.pos.x + background.size.w, background.pos.y + background.size.h},
        .p2 = {background.pos.x, background.pos.y + background.size.h},
        .sprite = '#',
        .color = {255, 255, 255},
    };

    if (init_scene(&select_scene) == error) return QUIT;
    INIT_INPUT;

    while (1) {
        if (
        draw_screen_borders(&select_scene, (color_t) {255, 255, 255}) == error ||
        draw_rectangle(&select_scene, background) == error ||
        draw_rectangle(&select_scene, cursor) == error ||
        draw_text_horizontal(&select_scene, quit) == error ||
        draw_text_horizontal(&select_scene, one_player) == error ||
        draw_text_horizontal(&select_scene, two_player) == error ||
        draw_text_horizontal(&select_scene, title) == error ||
        draw_line(&select_scene, outline_1) == error ||
        draw_line(&select_scene, outline_2) == error ||
        draw_line(&select_scene, outline_3) == error ||
        draw_line(&select_scene, outline_4) == error ||
        print_scene(&select_scene, false) == error)
        return QUIT;

        /*
        draw_screen_borders(&select_scene, (color_t) {255, 255, 255});
        draw_rectangle(&select_scene, background);
        draw_rectangle(&select_scene, cursor);
        draw_text_horizontal(&select_scene, quit);
        draw_text_horizontal(&select_scene, one_player);
        draw_text_horizontal(&select_scene, two_player);
        draw_text_horizontal(&select_scene, title);
        draw_line(&select_scene, outline_1);
        draw_line(&select_scene, outline_2);
        draw_line(&select_scene, outline_3);
        draw_line(&select_scene, outline_4);
        print_scene(&select_scene);
        */

        GET_INPUT;

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
        delay(1000 / fps);
    }
}

error_e main_game(selection sel) {
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
        .pos = {64, 18},
        .color = {255, 255, 255},
    };

    text_t instruction = {
        .str = "press w/s to move",
        .pos = {0, 0}
    };

    point_t ball_velocity = {((rand() % 2) == 1 ? 1 : -1), ((rand() % 2) == 1 ? 1 : -1)};
    
    rectangle_t paddle_1 = {
        .sprite = '#',
        .size = {1, 8},
        .pos = {2, 12},
        .color = {255, 0, 0},
    };

    rectangle_t paddle_2 = {
        .sprite = '#',
        .size = {1, 8},
        .pos = {125, 12},
        .color = {0, 0, 255},
    };
    
    rectangle_t score_1 = {
        .sprite = (char) score[0],
        .size = {1, 1},
        .pos = {4, 8},
        .color = {255, 0, 0},
    };

    rectangle_t score_2 = {
        .sprite = (char) score[1],
        .size = {1, 1},
        .pos = {123, 8},
        .color = {0, 0, 255},
    };


    if (init_scene(&scene) == error) return error;

    while (input != 'q') {
        GET_INPUT;

        if (
        draw_screen_borders(&scene, (color_t) {255, 255, 255}) == error ||
        draw_rectangle(&scene, paddle_1) == error ||
        draw_rectangle(&scene, paddle_2) == error ||
        draw_rectangle(&scene, ball) == error ||
        draw_rectangle(&scene, score_1) == error ||
        draw_rectangle(&scene, score_2) == error ||
        draw_text_horizontal(&scene, instruction) == error
        ) return error;
    
        score_1.sprite = (char) score[0];
        score_2.sprite = (char) score[1];

        turn = ball_velocity.x;

        switch (sel) {
            case ONE_P:
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
            break;
            case TWO_P:
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
            break;
            case QUIT:
                return success;
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
            if (clear_scene(&scene) == error) return error;
            clear_screen();
            draw_text_horizontal(&scene, (text_t) {
                .pos = {(int) (scene.size.w / 2) - (int) (strlen("Player 1 won!") / 2),
                        (int) (scene.size.h / 2)        
                },
                .str = "Payer 1 won!",
                .color = {255, 0, 0},
            });
            print_scene(&scene, false);
            delay(1000);
            break;
        } else if (score[1] == 53) {
            if (clear_scene(&scene) == error) return error;
            clear_screen();
            draw_text_horizontal(&scene, (text_t) {
                .pos = {(int) (scene.size.w / 2) - (int) (strlen("Player 2 won!") / 2),
                        (int) (scene.size.h / 2)        
                },
                .str = "Payer 2 won!",
                .color = {0, 0, 255},
            });
            print_scene(&scene, false);
            delay(1000);
            break;
        }

        //
        //add velocity vector to position
        ball.pos = add_points(ball.pos, ball_velocity);
        err = print_scene(&scene, false);
        if (err == error) return error;
        err = clear_scene(&scene);
        if (err == error) return error;
        delay(1000 / fps);
    }


    END_INPUT;
    free(scene.screen);
    free(scene.colors);
    scene.screen = NULL;
    scene.colors = NULL;

    return success;
}

int main() {
    clear_screen();
    selection sel = start_screen();
    main_game(sel);
    clear_screen();
}
