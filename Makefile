lib =       lib/utils.c lib/utils.h lib/keyboard.c lib/keyboard.h
showcases = showcases/ascii_img_showcase.c showcases/circle_showcase.c showcases/line_showcase.c showcases/rectangle_showcase.c showcases/text_showcase.c
examples =  examples/bouncing_ball.c examples/pong.c
flags = -std=c99 -D_POSIX_C_SOURCE=200809L -Wall -Wextra
default:
	@cat Makefile
bouncing_ball: $(lib) $(examples)
	cc -o bouncing_ball $(flags) examples/bouncing_ball.c lib/keyboard.c lib/utils.c include/cbmp.c
pong: $(lib) $(examples)
	cc -o pong examples/pong.c $(flags) lib/keyboard.c lib/utils.c include/cbmp.c
ascii_img_showcase: $(lib) $(showcases)
	cc -o ascii_img_showcase $(flags) showcases/ascii_img_showcase.c lib/keyboard.c lib/utils.c include/cbmp.c
circle_showcase: $(lib) $(showcases)
	cc -o circle_showcase $(flags) showcases/circle_showcase.c lib/keyboard.c lib/utils.c include/cbmp.c
line_showcase: $(lib) $(showcases)
	cc -o line_showcase $(flags) showcases/line_showcase.c lib/keyboard.c lib/utils.c include/cbmp.c
rectangle_showcase: $(lib) $(showcases)
	cc -o rectangle_showcase $(flags) showcases/rectangle_showcase.c lib/keyboard.c lib/utils.c include/cbmp.c
text_showcase: $(lib) $(showcases)
	cc -o text_showcase $(flags) showcases/text_showcase.c lib/keyboard.c lib/utils.c include/cbmp.c
