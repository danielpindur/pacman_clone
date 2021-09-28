CC=gcc
CFLAGS=-std=c99 -fsanitize=address -g -Wall -Wextra -Werror -o pacman
DEPS=app.c
LIBS=-lm -lSDL2

pacman: main.c
	$(CC) main.c $(DEPS) $(CFLAGS) $(LIBS)

run: pacman
	./pacman