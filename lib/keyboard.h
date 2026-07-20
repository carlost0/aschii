#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    char kb_input;
    int running;
} input_ctx_t;

extern char kb_input;
extern char input;
//extern pthread_mutex_t mutex;



#ifndef _WIN32

void * get_keyboard_input(void *args);
void enable_canonical_input();
void disable_canonical_input();


#define INIT_INPUT \
    pthread_t kb; \
    char input; \
    input_ctx_t *input_ctx = malloc(sizeof *input_ctx); \
    input_ctx->kb_input = 0; \
    input_ctx->running = 1; \
    pthread_mutex_init(&input_ctx->mutex, NULL); \
    disable_canonical_input(); \
    if (pthread_create(&kb, NULL, get_keyboard_input, input_ctx) != 0) \
        return 1;

#define GET_INPUT do { \
    pthread_mutex_lock(&input_ctx->mutex); \
    input = input_ctx->kb_input; \
    input_ctx->kb_input = 0; \
    pthread_mutex_unlock(&input_ctx->mutex); \
} while (0)

#define END_INPUT do { \
    pthread_mutex_lock(&input_ctx->mutex); \
    input_ctx->running = 0; \
    pthread_mutex_unlock(&input_ctx->mutex); \
    pthread_join(kb, NULL); \
    pthread_mutex_destroy(&input_ctx->mutex); \
    free(input_ctx); \
    enable_canonical_input(); \
} while (0)

#else

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <conio.h>

typedef struct KBThreadInput {
    CRITICAL_SECTION mutex;
    HANDLE thread;
    volatile LONG stop;
    volatile int kb_input; /* 0 = nothing */
} KBThreadInput;

static DWORD WINAPI kb_getchar_thread(void *arg) {
    KBThreadInput *ctx = (KBThreadInput*)arg;

    for (;;) {
        if (InterlockedCompareExchange(&ctx->stop, 0, 0) != 0) break;

        int ch = _getch(); /* blocks in this worker thread */

        EnterCriticalSection(&ctx->mutex);
        ctx->kb_input = ch;
        LeaveCriticalSection(&ctx->mutex);
    }
    return 0;
}

static void kb_input_init(KBThreadInput *ctx) {
    InitializeCriticalSection(&ctx->mutex);
    ctx->stop = 0;
    ctx->kb_input = 0;
    ctx->thread = CreateThread(NULL, 0, kb_getchar_thread, ctx, 0, NULL);
}

static void kb_input_end(KBThreadInput *ctx) {
    InterlockedExchange(&ctx->stop, 1);
    if (ctx->thread) {
        /* may still wait until a key arrives, because _getch blocks */
        WaitForSingleObject(ctx->thread, INFINITE);
        CloseHandle(ctx->thread);
        ctx->thread = NULL;
    }
    DeleteCriticalSection(&ctx->mutex);
}

static void kb_input_get(KBThreadInput *ctx, char *out_char) {
    EnterCriticalSection(&ctx->mutex);
    int v = ctx->kb_input;
    ctx->kb_input = 0;
    LeaveCriticalSection(&ctx->mutex);
    *out_char = (char)v;
}

/* ---- macros WITHOUT arguments ----
   Require these names to exist in the including .c file:
   - char input;
   - KBThreadInput kb_ctx;
*/

#define INIT_INPUT                                       \       
    char input;                                          \ 
    KBThreadInput kb_ctx;                                \
    kb_input_init(&kb_ctx);                              \

#define END_INPUT             kb_input_end(&kb_ctx);
#define GET_INPUT             kb_input_get(&kb_ctx, &input);
#endif //_WIN32
       
#endif //KEYBOARD_H
