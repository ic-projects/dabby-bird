CC      = gcc
LDFLAGS = -g -Wall
CFLAGS  = -Wall -g -D_XOPEN_SOURCE_EXTENDED -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -pedantic -O3


.SUFFIXES: .c .o

.PHONY: all clean

all: main main_snake object_list_tests

main: flappy_bird.o main.o object_list.o
	$(CC) $(ncursesw5-config CFLAGS) -o $@ $^ -lncursesw

#flappy_bird: flappy_bird.o object_list.o
#	$(CC) $(ncursesw5-config CFLAGS) -o $@ $^ -lncursesw

main_snake: snake.o main_snake.o object_list.o
	$(CC) $(ncursesw5-config CFLAGS) -o $@ $^ -lncursesw

object_list_tests: object_list_tests.o object_list.o
	$(CC) $(ncursesw5-config CFLAGS) -o $@ $^ -lncursesw

object_list_tests.o: object_list.h
flappy_bird.o: ascii_art.h object_list.h flappy_bird.h
object_list.o: object_list.h ascii_art.h
main.o: flappy_bird.h object_list.h ascii_art.h
snake.o: ascii_art.h object_list.h snake.h
main_snake.o: flappy_bird.h object_list.h ascii_art.h


clean:
	rm -rf $(wildcard *.o)
	rm -rf $(wildcard *.gch)
