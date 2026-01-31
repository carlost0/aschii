#ifndef SCREEN_H
#define SCREEN_H


typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color_t;

typedef struct {
    int w, h;
} box_t;

typedef struct {
    int x, y;
} point_t;

typedef struct {
    color_t *      colors;
    char *         screen;
    box_t          size;
} scene_t;

typedef struct {
    box_t         size;
    point_t       pos;
    color_t       color;
    char          sprite; // the object will be drawn with that character
} rectangle_t;

typedef struct {
    point_t       pos;
    color_t       color;
    int           radius;
    char          sprite; // the object will be drawn with that character
} circle_t;

typedef struct {
    point_t       p1; // first point
    point_t       p2; // second point
    color_t       color;
    char          sprite; // the object will be drawn with that character
} line_t;

typedef struct {
    point_t       pos;
    char *        str;
    color_t       color;
} text_t;

typedef struct {
    char *        sprite;
    point_t       pos;
    color_t       color;
    box_t         size;
} img_object_t;


void init_scene(scene_t *scene);                                            // initializes specified scene
void clear_scene(scene_t *scene);                                           // clears specified scene
void print_scene(scene_t *scene);                                           // prints specified scene to stdout

void draw_screen_borders(scene_t *scene, color_t color);                                   // draws cool borders to the specified scene
void draw_rectangle(scene_t *scene, rectangle_t rect);                      // draws a rectangle to the specified scene
void draw_text_horizontal(scene_t *scene, text_t text);                     // draws text horizonally to the specified scene
void draw_text_vertical(scene_t *scene, text_t text);                       // draws text vertically to the specified scene
void draw_line(scene_t *scene, line_t line);                                // draws a line to scpecified scene
void draw_point(scene_t *scene, point_t pos, char sprite, color_t color);   // draws sprite to the specified location on the specified scene
void draw_circle(scene_t *scene, circle_t circle);                          //draws a circle to the specified scene

void draw_img(scene_t *scene, img_object_t img);                            // draws an ascii image to specified scene
void img_to_ascii(char * img_path, img_object_t * img);                     // converts bitmap image to a string

int check_collision(rectangle_t box1, rectangle_t box2);                    // uses aabb to check collision between two boxes
point_t add_points(point_t p1, point_t p2);                                 // adds two points together: p1.x + p2.x; p1.y + p2.y
void delay(int ms);                                                         // delays specified time in millisecconds

extern char ascii_chars[];
#endif

