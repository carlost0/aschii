#include "utils.h"
#include "../include/cbmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


#ifdef _WIN32
#include <windows.h>
#endif //_WIN32
//#define SCENE_PIXEL_POS(x, y) (y * scene->size.w + x)


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

int random_num(int min, int max) {
    return rand() % (max - min + 1)
        + min;
}
point_t add_points(point_t v1, point_t v2) {
    point_t res;
    res.x = v1.x + v2.x;
    res.y = v1.y + v2.y;
    return res;
}

void delay(unsigned int ms) {
    #ifndef _WIN32

    clock_t start_time = clock();
    clock_t wait_time = ms * (CLOCKS_PER_SEC / 1000);

    while (clock() - start_time < wait_time);
    #else 

    Sleep(ms);
    #endif
}

error_e init_scene(scene_t *scene) {
    if (!scene) {
        perror("unable to init scene, scene is a null pointer");
        return error;
    }
    // FREE LATER!!!!
    scene->screen = (char *)    malloc((scene->size.w * scene->size.h) * sizeof(char));
    scene->colors = (color_t *) malloc((scene->size.w * scene->size.h) * sizeof(color_t));

    if (scene->screen == NULL || scene->colors == NULL) {
        perror("unable to init scene, failed to allocate screen memory");
        return error;
    }

    if (!(scene->screen && scene->colors)) {
        perror("unable to init scene, scene is a null pointer");
        return error;
    }

    memset(scene->screen, ' ', scene->size.w * scene->size.h);
    memset(scene->colors, ' ', scene->size.w * scene->size.h * sizeof(color_t));

    return success;
}

error_e clear_scene(scene_t *scene) {
    if (!(scene->screen && scene->colors)) {
        perror("unable to clear scene, scene is a null pointer");
        return error;
    }

    //printf("\e[H");
    memset(scene->screen, ' ', scene->size.w * scene->size.h);
    memset(scene->colors, ' ', scene->size.w * scene->size.h * sizeof(color_t));

    return success;
}

error_e print_scene(scene_t *scene, bool random) {
    if (!(scene->screen && scene->colors)) {
        perror("unable to print scene, scene is a null pointer");
        return error;
    }

    size_t cap = scene->size.w * scene->size.h * strlen("\x1b[32;2;255;255:255ma"); //worst case 
    char *buf = (char *) malloc(cap);

    if (!buf) {
        perror("unable to print scene, failed to allocate memory");
        return error;
    }

    printf("\x1b[H");
    char *ptr = buf;

    if (random) {
        for (int i = 0; i < scene->size.h; i++) {
            for (int j = 0; j < scene->size.w; j++) {
                // print pixel with color as 24bit ansi color code
                if (scene->screen[i * scene->size.w + j] == ' ')
                    ptr += sprintf(ptr, " ");
                else
                    ptr += sprintf(ptr, "\x1b[38;2;%u;%u;%um%c", \
                        scene->colors[i * scene->size.w + j].r,  \
                        scene->colors[i * scene->size.w + j].g,  \
                        scene->colors[i * scene->size.w + j].b,  \
                        ascii_chars[random_num(0, 70)]);
            }
            ptr += sprintf(ptr, "\n\r");
        }
    } else {
        for (int i = 0; i < scene->size.h; i++) {
            for (int j = 0; j < scene->size.w; j++) {
                // print pixel with color as 24bit ansi color code
                ptr += sprintf(ptr, "\x1b[38;2;%u;%u;%um%c", \
                    scene->colors[i * scene->size.w + j].r,  \
                    scene->colors[i * scene->size.w + j].g,  \
                    scene->colors[i * scene->size.w + j].b,  \
                    scene->screen[i * scene->size.w + j]);
            }
            ptr += sprintf(ptr, "\n\r");
        }
    }

    fwrite(buf, 1, (size_t) (ptr - buf), stdout);
    fflush(stdout);
    free(buf);

    return success;
}

void clear_screen() {
    printf("\x1b[1;1H\x1b[2J");
}

// Function optimized by duck.ai using Mistrall Small 3
error_e draw_screen_borders(scene_t *scene, color_t color) {
    if (!(scene->screen && scene->colors)) {
        perror("unable to draw screen borders, scene is a null pointer");
        return error;
    }

    // Draw the top and bottom borders
    for (int j = 0; j < scene->size.w; j++) {
        scene->screen[j] = '-';
        scene->screen[(scene->size.h - 1) * scene->size.w + j] = '-';
        scene->colors[j] = color;
        scene->colors[(scene->size.h - 1) * scene->size.w + j] = color;
    }

    for (int i = 1; i < scene->size.h - 1; i++) {
        scene->screen[i * scene->size.w] = '|';
        scene->screen[i * scene->size.w + scene->size.w - 1] = '|';
        scene->colors[i * scene->size.w] = color;
        scene->colors[i * scene->size.w + scene->size.w - 1] = color;
    }

    // Place the corners
    scene->screen[0] = '+';
    scene->screen[scene->size.w - 1] = '+';
    scene->screen[(scene->size.h - 1) * scene->size.w] = '+';
    scene->screen[(scene->size.h - 1) * scene->size.w + scene->size.w - 1] = '+';

    scene->colors[0] = color;
    scene->colors[scene->size.w - 1] = color;
    scene->colors[(scene->size.h - 1) * scene->size.w] = color;
    scene->colors[(scene->size.h - 1) * scene->size.w + scene->size.w - 1] = color;

    return success;
}

error_e draw_rectangle(scene_t *scene, rectangle_t rect) {
    if (!(scene->screen && scene->colors)) {
        perror("unable to draw rectangle, scene is a null pointer");
        return error;
    }

    if (rect.pos.x < 0 || rect.pos.x + rect.size.w > scene->size.w ||
        rect.pos.y < 0 || rect.pos.y + rect.size.h > scene->size.h) {
        perror("unable to draw rectangle, rectangle is out of bounds");
        return error;
    }

    for (int i = rect.pos.y; i < rect.pos.y + rect.size.h; i++) {
        for (int j = rect.pos.x; j < rect.pos.x + rect.size.w; j++) {
            scene->screen[i * scene->size.w + j] = rect.sprite;
            scene->colors[i * scene->size.w + j] = rect.color;
        }
    }

    return success;
}

error_e draw_text_horizontal(scene_t *scene, text_t text) {
    if (!(scene->screen && scene->colors)) {
        perror("unable to draw text, scene is a null pointer");
        return error;
    }

    if (text.pos.x + strlen(text.str) > scene->size.w) {
        perror("unable to draw text, text is out of bounds");
        return error;
    }

    if (text.pos.y < 0 || text.pos.y > scene->size.h) {
        perror("unable to draw text, text is out of bounds");
        return error;
    }

    for (int i = text.pos.x; i < text.pos.x + strlen(text.str); i++) { 
        scene->screen[text.pos.y * scene->size.w + i] = text.str[i - text.pos.x];
        scene->colors[text.pos.y * scene->size.w + i] = text.color;
    }

    return success;
}

error_e draw_text_vertical(scene_t *scene, text_t text) {
    if (!(scene->screen && scene->colors)) {
        perror("unable to draw text, scene is a null pointer");
        return error;
    }

    if (text.pos.y + strlen(text.str) > scene->size.h) {
        perror("unable to draw text, text is out of bounds");
        return error;
    }

    if (text.pos.x < 0 || text.pos.x > scene->size.w) {
        perror("unable to draw text, text is out of bounds");
        return error;
    }

    for (int i = text.pos.y; i < text.pos.y + strlen(text.str); i++) { 
        scene->screen[i * scene->size.w + text.pos.x] = text.str[i - text.pos.y];
        scene->colors[i * scene->size.w + text.pos.x] = text.color;
    }

    return success;
}

error_e draw_line(scene_t *scene, line_t line) {
    if (!(scene->screen && scene->colors)) {
        perror("unable to draw line, scene is a null pointer");
        return error;
    }

    if (line.p1.x < 0 || line.p1.x > scene->size.w || 
        line.p1.y < 0 || line.p1.y > scene->size.h) {
        perror("unable to draw line, p1 is out of bounds");
        return error;
    }

    if (line.p2.x < 0 || line.p2.x > scene->size.w || 
        line.p2.y < 0 || line.p2.y > scene->size.h) {
        perror("unable to draw line, p2 is out of bounds");
        return error;
    }

    //bresenhams line drawing algorithm
    int dx = abs(line.p2.x - line.p1.x);
    int dy = abs(line.p2.y - line.p1.y);
    int sx = (line.p1.x < line.p2.x) ? 1 : -1;
    int sy = (line.p1.y < line.p2.y) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        scene->screen[line.p1.y * scene->size.w + line.p1.x] = line.sprite;
        scene->colors[line.p1.y * scene->size.w + line.p1.x] = line.color;
        if (line.p1.x == line.p2.x && line.p1.y == line.p2.y) break;
        int err2 = err * 2;
        if (err2 > -dy) { err -= dy; line.p1.x += sx; }
        if (err2 < dx) { err += dx; line.p1.y += sy; }
    }

    return success;
}

error_e draw_pixel(scene_t *scene, point_t pos, char sprite, color_t color) {
    if (!(scene->screen && scene->colors)) {
        perror("unable to draw pixel, scene is a null pointer");
        return error;
    }

    if (pos.x < 0 || pos.x > scene->size.w || 
        pos.y < 0 || pos.y > scene->size.h) {
        perror("unable to draw pixel, pos is out of bounds");
        return error;
    }

    scene->screen[pos.y * scene->size.w + pos.x] = sprite;
    scene->colors[pos.y * scene->size.w + pos.x] = color;

    return success;
}

// thanks to https://www.youtube.com/@nobs_code for explaining this algorithm in https://www.youtube.com/watch?v=hpiILbMkF9w
error_e draw_circle(scene_t *scene, circle_t circle) {
    if (!(scene->screen && scene->colors)) {
        perror("unable to draw circle, scene is a null pointer");
        return error;
    }
    
    if (circle.pos.x - circle.radius < 0 || circle.pos.x + circle.radius > scene->size.w ||
        circle.pos.y - circle.radius < 0 || circle.pos.y + circle.radius > scene->size.h) {
        perror("unable to draw circle, circle is out of bounds");
        return error;
    }

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

        scene->colors[(circle.pos.y + y) * scene->size.w + (circle.pos.x + x)] = circle.color;
        scene->colors[(circle.pos.y + y) * scene->size.w + (circle.pos.x - x)] = circle.color;
        scene->colors[(circle.pos.y - y) * scene->size.w + (circle.pos.x + x)] = circle.color;
        scene->colors[(circle.pos.y - y) * scene->size.w + (circle.pos.x - x)] = circle.color;
        scene->colors[(circle.pos.y + x) * scene->size.w + (circle.pos.x + y)] = circle.color;
        scene->colors[(circle.pos.y + x) * scene->size.w + (circle.pos.x - y)] = circle.color;
        scene->colors[(circle.pos.y - x) * scene->size.w + (circle.pos.x + y)] = circle.color;
        scene->colors[(circle.pos.y - x) * scene->size.w + (circle.pos.x - y)] = circle.color;

        x++;

        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }

    return success;
}

/*
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
                scene->colors[screen_y * scene->size.w + screen_x] = ascii.color;
            }
        }
    }
}
*/


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

