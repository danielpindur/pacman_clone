CC=gcc
CFLAGS=-std=c99 -fsanitize=address -g -Wall -Wextra -o pacman #-Werror
DEPS=app.c game_controller.c graphics.c map.c player.c ui.c utils.c
LIBS=-lm -lSDL2

pacman: main.c
	$(CC) main.c $(DEPS) $(CFLAGS) $(LIBS) 2>error

run: pacman
	./pacman
