/**
 * @file object_list.h
 * @brief Data type for storing the state of the game.
 */
#ifndef object_list_h
#define object_list_h
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "ascii_art.h"

#define INITIAL_OBJECT_LIST_SIZE 20

typedef enum {
  bird,
  pipes,
  cloud,
  ground,
  snake_head,
  snake_body,
  snake_tail,
  snake_apple,
} type_t;

typedef struct {
  int16_t x;
  int16_t y;
} vector_t;

typedef struct object_list_elem {
  type_t type;
  vector_t point;
  vector_t velocity;
  vector_t acceleration;
  ascii_t *ascii;
  uint16_t depth;
  struct object_list_elem *prev;
} object_list_elem_t;

typedef struct {
  object_list_elem_t **array;
  uint16_t size;
  uint16_t max_size;
} object_list_t;

typedef void object_list_elem_function_t(object_list_elem_t *);

object_list_t *new_list(void);
void add_elem(object_list_t *list, object_list_elem_t *elem);
int is_covering (object_list_elem_t *elem, vector_t point);
char get_char_ascii(ascii_t *ascii, vector_t point);
char get_char_list(object_list_t *list, vector_t point);
void for_all(object_list_t *list, object_list_elem_function_t function);
void move_object(object_list_elem_t *elem);
void print_object(object_list_elem_t *elem);
int get_color(object_list_t *list, vector_t point);
void print_game(object_list_t *list, int width, int height);
void free_object_list(object_list_t *list);
void free_object_list_elem(object_list_elem_t *elem);
object_list_elem_t *get_elem(object_list_t *list, type_t type);

#endif
