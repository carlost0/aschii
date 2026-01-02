#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

void disable_canonical_input() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);      // get current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);    // disable canonical mode & echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void enable_canonical_input(struct termios *oldt) {
    tcsetattr(STDIN_FILENO, TCSANOW, oldt);  // restore old setting
}

char get_keyboard_input() {
    struct termios oldt;
    disable_canonical_input();
    char c = 0;
    read(STDIN_FILENO, &c, 1);
    enable_canonical_input(&oldt);
    return c;
}

/*
int main(void) {
    disable_canonical_input();
    char c = 0;
    while (1) {
        c = 0;
        read(STDIN_FILENO, &c, 1);
        if (c == '1') {
            enable_canonical_input();
            printf("12");
            disable_canonical_input();
        }
        if (c == 'q') {
            break;
        }
    }
    char c = 0;
    while (1) {
        c = get_keyboard_input();
        if (c == 'q') break;
    }
    printf("%c", c);
    printf("hello, world");
    return 0;
}
*/
