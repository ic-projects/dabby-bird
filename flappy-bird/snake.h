/**
 * @file snake.h
 * @brief Header file for snake.c.
 */
#ifndef flappy_bird_h
#define flappy_bird_h
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include "ascii_art.h"
#include "object_list.h"
#include <locale.h>

/** Width of the game. */
#define WIDTH 50
/** Height of the game. */
#define HEIGHT 50

int snake_hit(object_list_t *list);
object_list_t *init_game(void);
void move_snake(object_list_t *list, vector_t dir);
void render_game(object_list_t *list, vector_t dir);
void hit_apple(object_list_t *list);

#endif
