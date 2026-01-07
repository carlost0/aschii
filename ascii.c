#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "lib/cbmp.h"

typedef struct {
    box_t size;
    point_t pos;
    char * sprite;
} ascii_object;

static const char chars[71] =  {'$', '@', 'B', '%', '8', '&',
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

void img_to_ascii(char * img_path, ascii_object * ascii) {
    BMP * bmp = bopen(img_path);
    if (bmp == NULL) {
        printf("failed to open image");
        return;
    }

    char * res = (char *) malloc(ascii->size.w * ascii->size.h * sizeof(char));
    if (res == NULL) {
        bclose(bmp);
        printf("failed to allocate memory for image to ascii conversion");
        return;
    }

    unsigned char r, g, b;
    unsigned int width = get_width(bmp);
    unsigned int height = get_height(bmp);
    
    for (unsigned int x = 0; x < width && x < ascii->size.w; x++) {
        for (unsigned int y = 0; y < height && y < ascii->size.h; y++) {
            get_pixel_rgb(bmp, x, y, &r, &g, &b);
            double brightness = 0.3 * r + 0.59 * g + 0.11 * b;
            int index = (int)(70.0 / 255.0 * brightness);
            if (index >= 70) {
                index = 69; // Clamp index to prevent overflow
            }
            res[ascii->size.w * ascii->size.w - (y * ascii->size.w + x)] = chars[index];
        }
    }

    ascii->sprite = res;
    bclose(bmp); // Close BMP file
}

void put_img(ascii_object ascii) {
    for (int i = 0; i < ascii.size.h; i++) {
        for (int j = 0; j < ascii.size.w; j++) {
            int screenX = ascii.pos.x + j;
            int screenY = ascii.pos.y + i;
            if (screenX < screen.w && screenY < screen.h) { // Boundary check
                screen_arr[screenY * screen.w + screenX] = ascii.sprite[i * ascii.size.w + j];
            }
        }
    }
}

