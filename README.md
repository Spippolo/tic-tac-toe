# Tic Tac Toe

![Tic Tac Toe](http://f.cl.ly/items/1B2C1R0d080L0o0l2L2L/tic-tac-toe-screenshot.png)

## Requirements

To compile the software you need GCC and the SDL2 and SDL2_image libraries.

On Mac OSX, GCC comes with XCode and you can install the libraries with brew:

```
brew install sdl2 sdl2_image
```

## Usage

```
~$ make
gcc -c `pkg-config --cflags sdl2 SDL2_image` main.c
gcc `pkg-config --libs sdl2 SDL2_image` -lm main.o -o main.out
~$ ./main.out
```

Enjoy!
