#include <stdio.h>

int main() {
    printf("\x1b[31mRED\x1b[0m\n");
    //printf("\x1b[H\x1b[2J"); // home + clear
    fflush(stdout);
    return 0;
}
