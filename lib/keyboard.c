#include <unistd.h>
#include <termios.h>
#include "keyboard.h"

struct termios oldt;
char kb_input = 0;

void disable_canonical_input() {
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);      // get current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);    // disable canonical mode & echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void enable_canonical_input() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // restore old setting
}

void* get_keyboard_input(void *arg) {
    //disable_canonical_input();  // Enable non-canonical mode
    read(STDIN_FILENO, &kb_input, 1);  // Read one character
    //enable_canonical_input();    // Restore canonical mode
    return NULL;
}

