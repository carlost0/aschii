#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int w, h;
} box_t;

typedef struct {
    int x, y;
} point_t;

typedef struct {
    box_t size;
    point_t pos;
    char sprite;
} object_t;

typedef struct {
    point_t pos;
    char * str;
} text_t;

struct timespec ts;

char* screen_arr;
box_t screen;

point_t add_points(point_t v1, point_t v2) {
    point_t res;
    res.x = v1.x + v2.x;
    res.y = v1.y + v2.y;
    return res;
}

void delay(int ms) {
    ts.tv_sec = 0;
    ts.tv_nsec = ms * 1000;
    nanosleep(&ts, NULL); //lsp might give error, compile with -D_POSIX_C_SOURCE=200809L
}
void init_screen(char c) {
    // FREE LATER!!!!
    screen_arr = (char *) malloc((screen.w * screen.h) * sizeof(char));
    memset(screen_arr, c, screen.w * screen.h);
}

void clear_screen() {
    //remove line to get rid of flickering, can cause wierd scrolling
    printf("\e[1;1H\e[2J");
    memset(screen_arr, ' ', screen.w * screen.h);

}
void draw_screen() {
    if (screen_arr == NULL) return;

    for (int i = 0; i < screen.h; i++) {
        for (int j = 0; j < screen.w; j++) {
            printf("%c", screen_arr[i * screen.w + j]);
        }
        printf("\n");
    }
}

// Function optimized by duck.ai using Mistrall Small 3
void put_screen_borders() {
    // Draw the top and bottom borders
    for (int j = 0; j < screen.w; j++) {
        screen_arr[j] = '-';
        screen_arr[(screen.h - 1) * screen.w + j] = '-';
    }

    // Draw the left and right borders
    for (int i = 1; i < screen.h - 1; i++) {
        screen_arr[i * screen.w] = '|';
        screen_arr[i * screen.w + screen.w - 1] = '|';
    }

    // Place the corners
    screen_arr[0] = '+';
    screen_arr[screen.w - 1] = '+';
    screen_arr[(screen.h - 1) * screen.w] = '+';
    screen_arr[(screen.h - 1) * screen.w + screen.w - 1] = '+';
}

void put_object(object_t obj) {
    /*
    1. put point as obj.pos into screen_arr
    2. extend point by width and height
    */
    for (int i = obj.pos.y; i < obj.pos.y + obj.size.h; i++) {
        for (int j = obj.pos.x; j < obj.pos.x + obj.size.w; j++) {
            screen_arr[i * screen.w + j] = obj.sprite;
        }
    }
}

void put_text(text_t text) {
    for (int i = text.pos.x; i < text.pos.x + strlen(text.str); i++) { 
        if (text.pos.x + strlen(text.str) != NULL) screen_arr[text.pos.y * screen.w + i] = text.str[i - text.pos.x];
    }
}

// function stolen from https://peerdh.com/blogs/programming-insights/implementing-aabb-collision-detection-algorithms-in-c-for-2d-sprite-based-games-1
int check_collision(object_t box1, object_t box2) {
    // Check if box1 is to the left of box2
    if (box1.pos.x + box1.size.w < box2.pos.x) return 0;
    // Check if box1 is to the right of box2
    if (box1.pos.x > box2.pos.x + box2.size.w) return 0;
    // Check if box1 is above box2
    if (box1.pos.y + box1.size.h < box2.pos.y) return 0;
    // Check if box1 is below box2
    if (box1.pos.y > box2.pos.y + box2.size.h) return 0;

    // If none of the above, a collision has occurred
    return 1;
}
