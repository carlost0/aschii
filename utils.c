#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "types.h"

struct timespec ts;

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
    if (screen_arr == NULL) {perror("failed to allocate screen memory"); return;}
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

void put_rectangle(rectangle_t rect) {
    for (int i = rect.pos.y; i < rect.pos.y + rect.size.h; i++) {
        for (int j = rect.pos.x; j < rect.pos.x + rect.size.w; j++) {
            screen_arr[i * screen.w + j] = rect.sprite;
        }
    }
}

void put_text_horizontal(text_t text) {
    for (int i = text.pos.x; i < text.pos.x + strlen(text.str); i++) { 
        if (text.pos.x + strlen(text.str) != NULL) {
            screen_arr[text.pos.y * screen.w + i] = text.str[i - text.pos.x];
        }
    }
}

void put_text_vertical(text_t text) {
    for (int i = text.pos.y; i < text.pos.y + strlen(text.str); i++) { 
        if (text.pos.y + strlen(text.str) != NULL) {
            screen_arr[i * screen.w + text.pos.x] = text.str[i - text.pos.y];
        }
    }
}

void put_line(line_t line) {
    //bresenhams line drawing algorithm
    int dx = abs(line.p2.x - line.p1.x);
    int dy = abs(line.p2.y - line.p1.y);
    int sx = (line.p1.x < line.p2.x) ? 1 : -1;
    int sy = (line.p1.y < line.p2.y) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        screen_arr[line.p1.y * screen.w + line.p1.x] = line.sprite;
        if (line.p1.x == line.p2.x && line.p1.y == line.p2.y) break;
        int err2 = err * 2;
        if (err2 > -dy) { err -= dy; line.p1.x += sx; }
        if (err2 < dx) { err += dx; line.p1.y += sy; }
    }
}

void put_point(point_t pos, char sprite) {
    screen_arr[pos.y * screen.w + pos.x] = sprite;
}

// thanks to https://www.youtube.com/@nobs_code for explaining this algorithm in https://www.youtube.com/watch?v=hpiILbMkF9w
void put_circle(circle_t circle) {
    int x = 0;
    int y = circle.radius;
    int p = 1 - circle.radius;

    while (x <= y) {
        screen_arr[(circle.pos.y + y) * screen.w + (circle.pos.x + x)] = circle.sprite;
        screen_arr[(circle.pos.y + y) * screen.w + (circle.pos.x - x)] = circle.sprite;
        screen_arr[(circle.pos.y - y) * screen.w + (circle.pos.x + x)] = circle.sprite;
        screen_arr[(circle.pos.y - y) * screen.w + (circle.pos.x - x)] = circle.sprite;
        screen_arr[(circle.pos.y + x) * screen.w + (circle.pos.x + y)] = circle.sprite;
        screen_arr[(circle.pos.y + x) * screen.w + (circle.pos.x - y)] = circle.sprite;
        screen_arr[(circle.pos.y - x) * screen.w + (circle.pos.x + y)] = circle.sprite;
        screen_arr[(circle.pos.y - x) * screen.w + (circle.pos.x - y)] = circle.sprite;

        x++;

        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}

// function stolen from https://peerdh.com/blogs/programming-insights/implementing-aabb-collision-detection-algorithms-in-c-for-2d-sprite-based-games-1
int check_collision(rectangle_t box1, rectangle_t box2) {
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
