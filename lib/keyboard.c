#include "keyboard.h"

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#include <pthread.h>

struct termios oldt;
char kb_input = 0;
int input_running;

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

void *get_keyboard_input(void *arg) {
    input_ctx_t *ctx = arg;
    while (ctx->running) {
        char c;
        size_t n = read(STDIN_FILENO, &c, 1);
        if (n == 1) {
            pthread_mutex_lock(&ctx->mutex);
            ctx->kb_input = c;
            pthread_mutex_unlock(&ctx->mutex);
        }
    }
    return NULL;
}
#else


/*
DWORD WINAPI kb_getchar_thread(void *arg) {
    KBThreadInput *ctx = (KBThreadInput*)arg;

    for (;;) {
        if (InterlockedCompareExchange(&ctx->stop, 0, 0) != 0) break;


        EnterCriticalSection(&ctx->mutex);
        ctx->kb_input = ch;
        LeaveCriticalSection(&ctx->mutex);
    }
    return 0;
}

void kb_input_init(KBThreadInput *ctx) {
    InitializeCriticalSection(&ctx->mutex);
    ctx->stop = 0;
    ctx->kb_input = 0;
    ctx->thread = CreateThread(NULL, 0, kb_getchar_thread, ctx, 0, NULL);
}

void kb_input_end(KBThreadInput *ctx) {
    InterlockedExchange(&ctx->stop, 1);
    if (ctx->thread) {
        WaitForSingleObject(ctx->thread, INFINITE);
        CloseHandle(ctx->thread);
        ctx->thread = NULL;
    }
    DeleteCriticalSection(&ctx->mutex);
}

void kb_input_get(KBThreadInput *ctx, char *out_char) {
    EnterCriticalSection(&ctx->mutex);
    int v = ctx->kb_input;
    ctx->kb_input = 0;
    LeaveCriticalSection(&ctx->mutex);
    *out_char = (char)v;
}
*/
#endif
