bouncing_ball:
	cc -o bouncing_ball -std=c99 -D_POSIX_C_SOURCE=200809L examples/bouncing_ball.c lib/keyboard.c lib/utils.c include/cbmp.c
pong:
	cc -o pong -std=c99 -D_POSIX_C_SOURCE=200809L examples/pong.c lib/keyboard.c lib/utils.c include/cbmp.c
ascii_img_showcase:
	cc -o ascii_img_showcase -std=c99 -D_POSIX_C_SOURCE=200809L showcases/ascii_img_showcase.c lib/keyboard.c lib/utils.c include/cbmp.c
circle_showcase:
	cc -o circle_showcase -std=c99 -D_POSIX_C_SOURCE=200809L showcases/circle_showcase.c lib/keyboard.c lib/utils.c include/cbmp.c
line_showcase:
	cc -o line_showcase -std=c99 -D_POSIX_C_SOURCE=200809L showcases/line_showcase.c lib/keyboard.c lib/utils.c include/cbmp.c
rectangle_showcase:
	cc -o rectangle_showcase -std=c99 -D_POSIX_C_SOURCE=200809L showcases/rectangle_showcase.c lib/keyboard.c lib/utils.c include/cbmp.c
text_showcase:
	cc -o text_showcase -std=c99 -D_POSIX_C_SOURCE=200809L showcases/text_showcase.c lib/keyboard.c lib/utils.c include/cbmp.c

