#ifndef KEYBOARD_H
#define KEYBOARD_H

extern char kb_input;
extern char input;

#define INIT_INPUT                                       \
    char input;                                          \
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   \
    pthread_t kb;                                        \
    pthread_mutex_init(&mutex, NULL);                    \
    disable_canonical_input();

#define END_INPUT                                        \
    enable_canonical_input();                            \
    pthread_mutex_destroy(&mutex);                       \

#define GET_INPUT do {                                   \
    pthread_create(&kb, NULL, get_keyboard_input, NULL); \
    pthread_mutex_lock(&mutex);                          \
    input = kb_input;                                    \
    pthread_mutex_unlock(&mutex);                        \
    kb_input = 0;                                        \
} while (0);

void * get_keyboard_input(void *args);
void enable_canonical_input();
void disable_canonical_input();

#endif //KEYBOARD_H
