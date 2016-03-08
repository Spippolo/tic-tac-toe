L_OPTS=`pkg-config --libs sdl2`
C_OPTS=`pkg-config --cflags sdl2`
CC=gcc

all: main.o
	$(CC) $(L_OPTS) main.o -o main.out

main.o: main.c
	$(CC) -c $(C_OPTS) main.c

clean:
	rm *.o *.out
