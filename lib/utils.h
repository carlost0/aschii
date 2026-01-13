#ifndef SCREEN_H
#define SCREEN_H

typedef struct {
    int w, h;
} box_t;

typedef struct {
    int x, y;
} point_t;

typedef struct {
    char * screen;
    box_t size;
} scene_t;

typedef struct {
    box_t size;
    point_t pos;
    char sprite; // the object will be drawn with that character
} rectangle_t;

typedef struct {
    point_t pos;
    int radius;
    char sprite; // the object will be drawn with that character
} circle_t;

typedef struct {
    point_t p1;
    point_t p2;
    char sprite; // the object will be drawn with that character
} line_t;

typedef struct {
    point_t pos;
    char * str;
} text_t;

typedef struct {
    char * sprite;
    point_t pos;
    box_t size;
} img_object_t;


void init_scene(scene_t *scene); 
void clear_scene(scene_t *scene); 
void print_scene(scene_t *scene); 

void draw_screen_borders(scene_t *scene); 
void draw_rectangle(scene_t *scene, rectangle_t rect); 
void draw_text_horizontal(scene_t *scene, text_t text); 
void draw_text_vertical(scene_t *scene, text_t text); 
void draw_line(scene_t *scene, line_t line); 
void draw_point(scene_t *scene, point_t pos, char sprite); // the point will be drawn with sprite
void draw_circle(scene_t *scene, circle_t circle); 

void draw_img(scene_t *scene, img_object_t ascii);
void img_to_ascii(char * img_path, img_object_t * ascii);

int check_collision(rectangle_t box1, rectangle_t box2);
point_t add_points(point_t v1, point_t v2); 
void delay(int ms); 

extern char ascii_chars[];
#endif
