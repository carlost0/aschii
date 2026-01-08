#ifndef SCREEN_H
#define SCREEN_H

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
} rectangle_t;

typedef struct {
    point_t pos;
    char * str;
} text_t;

typedef struct {
    point_t p1;
    point_t p2;
    char sprite;
} line_t;

char* screen_arr;
box_t screen;

#endif
