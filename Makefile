L_OPTS=`pkg-config --libs sdl2 SDL2_image`
C_OPTS=`pkg-config --cflags sdl2 SDL2_image`
CC=gcc

all: main.o
	$(CC) $(L_OPTS) -lm main.o -o main.out

main.o: main.c
	$(CC) -c $(C_OPTS) main.c

clean:
	rm *.o *.out
