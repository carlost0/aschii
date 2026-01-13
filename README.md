# aschii
simple c framework for ascii gamedevelopment on linux
## docs
### `lib/utils.h`
#### types
types are defined in the `lib/utils.h` file, i recommend you checking it out
#### understanding `scene_t`:
- `scene.screen` is a one-dimensional array of type `char *`.
- To access a point at `x = 10, y = 3`, you would do: `scene.screen[y * scene.screen.w + x];`
#### functions
functions are declared and commented in `lib/utils.h`
### `lib/keyboard.h`
currently, input is taken from stdin on a seperate thread
- to initialize input, use the `INIT_INPUT` macro
- to get input use the `GET_INPUT` macro which stores the input to a variable of type char called `input`
- don't forget to end the input system once you're done using it with the `END_INPUT`
---
for examples, check out the `examples/` and `showcases/` directory
## todo
- add good documentation
- add sound support
- add colors
- change input system so it doesnt read from stdin anymore
## !todo
- add windows support
- start capitalizing words
## acknowledgements
- used some generative AI to fix bugs and optimize code
- used mattflows beautiful bitmap c library: https://github.com/mattflow/cbmp
