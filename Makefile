
lib =       lib/utils.c lib/utils.h lib/keyboard.c lib/keyboard.h
showcases = showcases/ascii_img_showcase.c showcases/circle_showcase.c showcases/line_showcase.c showcases/rectangle_showcase.c showcases/text_showcase.c
examples =  examples/bouncing_ball.c examples/pong.c

CFLAGS = -std=c99 -Wall -Wextra
CC = x86_64-w64-mingw32-gcc

clean: 
	-rm *.a *.o *example *showcase
aschii:
	$(CC) $(CFLAGS) -c lib/*.c
	ar -cvq aschii.a *.o 

bouncing_ball: $(lib) $(examples)
	$(CC) -o bouncing_ball_example $(CFLAGS) examples/bouncing_ball.c aschii.a
pong: $(lib) $(examples)
	$(CC) -o pong_example $(CFLAGS) examples/pong.c aschii.a
circle_showcase: $(lib) $(showcases)
	$(CC) -o circle_showcase $(CFLAGS) showcases/circle_showcase.c lib/keyboard.caschii.a
line_showcase: $(lib) $(showcases)
	$(CC) -o line_showcase $(CFLAGS) showcases/line_showcase.c lib/keyboard.c aschii.a
rectangle_showcase: $(lib) $(showcases)
	$(CC) -o rectangle_showcase $(CFLAGS) showcases/rectangle_showcase.c aschii.a
text_showcase: $(lib) $(showcases)
	$(CC) -o text_showcase $(CFLAGS) showcases/text_showcase.c lib/keyboard.c aschii.a
