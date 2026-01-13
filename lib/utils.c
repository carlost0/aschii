#include "utils.h"
#include "../include/cbmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

struct timespec ts;

char ascii_chars[71] =  {'$', '@', 'B', '%', '8', '&',
                'W', 'M', '#', '*', 'o', 'a',
                'h', 'k', 'b', 'd', 'p', 'q',
                'w', 'm', 'Z', 'O', '0', 'Q',
                'L', 'C', 'J', 'U', 'Y', 'X',
                'z', 'c', 'v', 'u', 'n', 'x',
                'r', 'j', 'f', 't', '/', '\\',
                '|', '(', ')', '1', '{', '}',
                '[', ']', '?', '-', '_', '+',
                '~', '<', '>', 'i', '!', 'l',
                'I', ';', ':', ',', '"', '^',
                '`', '\'', '.', ' '};

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

void init_scene(scene_t *scene) {
    // FREE LATER!!!!
    scene->screen = (char *) malloc((scene->size.w * scene->size.h) * sizeof(char));
    if (scene->screen == NULL) {perror("failed to allocate screen memory"); return;}
    memset(scene->screen, ' ', scene->size.w * scene->size.h);
}

void clear_scene(scene_t *scene) {
    //remove line to get rid of flickering, can cause wierd scrolling
    printf("\e[1;1H\e[2J");
    memset(scene->screen, ' ', scene->size.w * scene->size.h);

}
void print_scene(scene_t *scene) {
    if (scene->screen == NULL) return;

    for (int i = 0; i < scene->size.h; i++) {
        for (int j = 0; j < scene->size.w; j++) {
            printf("%c", scene->screen[i * scene->size.w + j]);
        }
        printf("\n");
    }
}

// Function optimized by duck.ai using Mistrall Small 3
void draw_screen_borders(scene_t *scene) {
    // Draw the top and bottom borders
    for (int j = 0; j < scene->size.w; j++) {
        scene->screen[j] = '-';
        scene->screen[(scene->size.h - 1) * scene->size.w + j] = '-';
    }

    // Draw the left and right borders
    /*
    for (int i = 1; i < screen.h - 1; i++) {
        scene->screen[i * scene.size.w] = '|';
        scene->screen[i * scene.size.w + screen.w - 1] = '|';
    }
    */
    for (int i = 1; i < scene->size.h - 1; i++) {
        scene->screen[i * scene->size.w] = '|';
        scene->screen[i * scene->size.w + scene->size.w - 1] = '|';
    }

    // Place the corners
    scene->screen[0] = '+';
    scene->screen[scene->size.w - 1] = '+';
    scene->screen[(scene->size.h - 1) * scene->size.w] = '+';
    scene->screen[(scene->size.h - 1) * scene->size.w + scene->size.w - 1] = '+';
}

void draw_rectangle(scene_t *scene, rectangle_t rect) {
    for (int i = rect.pos.y; i < rect.pos.y + rect.size.h; i++) {
        for (int j = rect.pos.x; j < rect.pos.x + rect.size.w; j++) {
            scene->screen[i * scene->size.w + j] = rect.sprite;
        }
    }
}

void draw_text_horizontal(scene_t *scene, text_t text) {
    for (int i = text.pos.x; i < text.pos.x + strlen(text.str); i++) { 
        if (text.pos.x + strlen(text.str) != NULL) {
            scene->screen[text.pos.y * scene->size.w + i] = text.str[i - text.pos.x];
        }
    }
}

void draw_text_vertical(scene_t *scene, text_t text) {
    for (int i = text.pos.y; i < text.pos.y + strlen(text.str); i++) { 
        if (text.pos.y + strlen(text.str) != NULL) {
            scene->screen[i * scene->size.w + text.pos.x] = text.str[i - text.pos.y];
        }
    }
}

void draw_line(scene_t *scene, line_t line) {
    //bresenhams line drawing algorithm
    int dx = abs(line.p2.x - line.p1.x);
    int dy = abs(line.p2.y - line.p1.y);
    int sx = (line.p1.x < line.p2.x) ? 1 : -1;
    int sy = (line.p1.y < line.p2.y) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        scene->screen[line.p1.y * scene->size.w + line.p1.x] = line.sprite;
        if (line.p1.x == line.p2.x && line.p1.y == line.p2.y) break;
        int err2 = err * 2;
        if (err2 > -dy) { err -= dy; line.p1.x += sx; }
        if (err2 < dx) { err += dx; line.p1.y += sy; }
    }
}

void draw_point(scene_t *scene, point_t pos, char sprite) {
    scene->screen[pos.y * scene->size.w + pos.x] = sprite;
}

// thanks to https://www.youtube.com/@nobs_code for explaining this algorithm in https://www.youtube.com/watch?v=hpiILbMkF9w
void draw_circle(scene_t *scene, circle_t circle) {
    int x = 0;
    int y = circle.radius;
    int p = 1 - circle.radius;

    while (x <= y) {
        scene->screen[(circle.pos.y + y) * scene->size.w + (circle.pos.x + x)] = circle.sprite;
        scene->screen[(circle.pos.y + y) * scene->size.w + (circle.pos.x - x)] = circle.sprite;
        scene->screen[(circle.pos.y - y) * scene->size.w + (circle.pos.x + x)] = circle.sprite;
        scene->screen[(circle.pos.y - y) * scene->size.w + (circle.pos.x - x)] = circle.sprite;
        scene->screen[(circle.pos.y + x) * scene->size.w + (circle.pos.x + y)] = circle.sprite;
        scene->screen[(circle.pos.y + x) * scene->size.w + (circle.pos.x - y)] = circle.sprite;
        scene->screen[(circle.pos.y - x) * scene->size.w + (circle.pos.x + y)] = circle.sprite;
        scene->screen[(circle.pos.y - x) * scene->size.w + (circle.pos.x - y)] = circle.sprite;

        x++;

        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}

void img_to_ascii(char * img_path, img_object_t * img) {
    BMP * bmp = bopen(img_path);
    if (bmp == NULL) {
        perror("failed to open image");
        return;
    }

    char * res = (char *) malloc(img->size.w * img->size.h * sizeof(char));
    if (res == NULL) {
        bclose(bmp);
        perror("failed to allocate memory for image to ascii conversion");
        return;
    }

    unsigned char r, g, b;
    unsigned int width = get_width(bmp);
    unsigned int height = get_height(bmp);
    
    for (unsigned int x = 0; x < width && x < img->size.w; x++) {
        for (unsigned int y = 0; y < height && y < img->size.h; y++) {
            get_pixel_rgb(bmp, x, y, &r, &g, &b);
            double brightness = 0.3 * r + 0.59 * g + 0.11 * b;
            int index = (int)(70.0 / 255.0 * brightness);
            if (index >= 70) {
                index = 69; // Clamp index to prevent overflow
            }
            res[img->size.w * img->size.w - (y * img->size.w + x)] = ascii_chars[index];
        }
    }

    img->sprite = res;
    bclose(bmp); // Close BMP file
}

void draw_img(scene_t *scene, img_object_t ascii) {
    for (int i = 0; i < ascii.size.h; i++) {
        for (int j = 0; j < ascii.size.w; j++) {
            int screen_x = ascii.pos.x + j;
            int screen_y = ascii.pos.y + i;
            if (screen_x < scene->size.w && screen_y < scene->size.h) { // Boundary check
                scene->screen[screen_y * scene->size.w + screen_x] = ascii.sprite[i * ascii.size.w + j];
            }
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
