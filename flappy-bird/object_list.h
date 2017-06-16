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

/** Initial size of the object list array. */
#define INITIAL_OBJECT_LIST_SIZE 20

/**
 * @brief An enum to store the type of the object.
 */
typedef enum {
  /** Bird. */
  bird,
  /** Pipes. */
  pipes,
  /** Cloud. */
  cloud,
  /** Ground. */
  ground,
  /** Snake Head. */
  snake_head,
  /** Snake Body. */
  snake_body,
  /** Snake Tail. */
  snake_tail,
  /** Snake Apple. */
  snake_apple,
} type_t;

/**
 * @brief A vector pair struct.
 */
typedef struct {
  /** X value. */
  int16_t x;
  /** Y value. */
  int16_t y;
} vector_t;

/**
 * @brief A struct to store the state of an object.
 */
typedef struct object_list_elem {
  /** Type of the object. */
  type_t type;
  /** Position of the object. */
  vector_t point;
  /** Velocity of the object. */
  vector_t velocity;
  /** Acceleration of the object. */
  vector_t acceleration;
  /** A pointer to an ascii struct. */
  ascii_t *ascii;
  /** Depth of the object. */
  uint16_t depth;
  /** Previous object (used in snake). */
  struct object_list_elem *prev;
} object_list_elem_t;

/**
 * @brief A struct to store a list of objects.
 */
typedef struct {
  /** Array of pointers to objects. */
  object_list_elem_t **array;
  /** Size of the array. */
  uint16_t size;
  /** Size currently allocated for the array. */
  uint16_t max_size;
} object_list_t;

/** A function that can be applied to all of the list.*/
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
