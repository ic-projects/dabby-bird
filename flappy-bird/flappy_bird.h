/**
 * @file flappy_bird.h
 * @brief Header file for flappy bird
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

/** Width of the game, in characters. */
#define WIDTH 300
/** Height of the game, in characters. */
#define HEIGHT 100

void move_pipes(object_list_elem_t *elem);
void flap(object_list_elem_t *elem);
int bird_coll(object_list_t *list);
object_list_t *init_game(void);
void render_game(object_list_t *list);

#endif
