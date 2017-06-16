/**
 * @file object_list.c
 * @brief Functions for using object list.
 */
#include "object_list.h"

/**
 * @brief Returns an initialised object list.
 *
 * @returns Initialised object list.
 */
object_list_t *new_list(void) {
  object_list_t *list = malloc(sizeof(object_list_t));
  if (!list) {
    perror("Unable to allocate memory for new list");
    exit(EXIT_FAILURE);
  }
  list->array = malloc(sizeof(object_list_elem_t *) * INITIAL_OBJECT_LIST_SIZE);
  if (!list->array) {
    perror("Unable to allocate memory for new list");
    exit(EXIT_FAILURE);
  }
  list->size = 0;
  list->max_size = INITIAL_OBJECT_LIST_SIZE;
  return list;
}

/**
 * @brief Adds a element to an object list.
 *
 * @param list Object list to add element to.
 * @param elem Object list elem to add to the list.
 */
void add_elem(object_list_t *list, object_list_elem_t *elem) {
  if (list->size >= list->max_size) {
    list->max_size *= 2;
    list->array = realloc(list->array, sizeof(object_list_elem_t *) * list->max_size);
    if (!list->array) {
      perror("Unable to reallocate memory for object list");
      exit(EXIT_FAILURE);
    }
  }
  list->array[list->size] = elem;
  list->size++;
}

/**
 * @brief Returns if the object is covering the point.
 *
 * @param elem Object to check.
 * @point Position to check.
 * @returns 1 if covering, 0 otherwise.
 */
int is_covering (object_list_elem_t *elem, vector_t point) {
  if ((elem->point.x <= point.x) && ((elem->point.x + elem->ascii->width) > point.x)) {
    if ((elem->point.y <= point.y) && ((elem->point.y + elem->ascii->height) > point.y)) {
      return 1;
    }
  }
  return 0;
}

/**
 * @brief Returns the char at a given position in the ascii ascii_art.
 *
 * @param ascii A struct storing the ascii art.
 * @param point Position in the ascii.
 * @returns Char at the position.
 */
char get_char_ascii(ascii_t *ascii, vector_t point) {
  return ascii->ascii[point.y * ascii->width + point.x];
}

/**
 * @brief Returns the char at a given position.
 *
 * @param list Object list.
 * @param point Position of the char.
 * @returns Char at given position.
 */
char get_char_list(object_list_t *list, vector_t point) {
  for (int i = 0; i < list->size; i++) {
    if (is_covering(list->array[i], point)) {
      point.x -= list->array[i]->point.x;
      point.y -= list->array[i]->point.y;
      return get_char_ascii(list->array[i]->ascii, point);
    }
  }
  return EMPTY_SPACE;
}

/**
 * @brief Applies a function to a object list.
 * 
 * @param list Object list to apply the function to.
 * @param function Function to apply.
 */
void for_all(object_list_t *list, object_list_elem_function_t function) {
  for (int i = 0; i < list->size; i++) {
    function(list->array[i]);
  }
}

/**
 * @brief Moves an object and updates its velocity.
 *
 * Is a object_list_elem_function_t so can be called with for_all.
 * @param elem Object to move.
 */
void move_object(object_list_elem_t *elem) {
  elem->point.x += elem->velocity.x;
  elem->point.y += elem->velocity.y;
  elem->velocity.x += elem->acceleration.x;
  elem->velocity.y += elem->acceleration.y;
}

/**
 * @brief Prints out an object.
 *
 * Is a object_list_elem_function_t so can be called with for_all.
 * @param elem Object to print.
 */
void print_object(object_list_elem_t *elem) {
  printf("\nObject type: %d\n", elem->type);
  printf("Has:\n");
  printf("Depth: %d\n", elem->depth);
  printf("Position: (%d, %d)\n", elem->point.x, elem->point.y);
  //printf("Velocity: (%d, %d)\n", elem->velocity.x, elem->velocity.y);
  printf("Char at 0, 0: %c\n", get_char_ascii(elem->ascii, (vector_t) {0, 0}));
}

/**
 * @brief Gets the first element with a matching type in the list.
 *
 * @param list List to look through.
 * @param type Type of object to fine.
 * @returns Object with matching type.
 */
object_list_elem_t *get_elem(object_list_t *list, type_t type) {
  for (int i = 0; i < list->size; i++) {
    if (list->array[i]->type == type) {
      return list->array[i];
    }
  }
  return NULL;
}

/**
 * @brief Returns the colour for a paticular position.
 *
 * @param list The current game state.
 * @param point The position to return the colour of.
 * @returns The colour of the position.
 */
int get_color(object_list_t *list, vector_t point) {
  for (int i = 0; i < list->size; i++) {
    if (is_covering(list->array[i], point)) {
      return list->array[i]->ascii->color;
    }
  }
  return 1;
}

/**
 * @brief Prints the game.
 *
 * @param list The current game state.
 * @param width Width of the screen being used.
 * @param height Height of the screen being used.
 */
void print_game(object_list_t *list, int width, int height) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      vector_t point = {j, i};
      int color = get_color(list, point);
      char c = get_char_list(list, point);
      if (c == ' ') {
        color*=2;
      }
      attron(COLOR_PAIR(color));
      addch(c);
    }
    printw("\n");
  }
}

/**
 * @brief Free's all data used in the object list.
 *
 * @param list Object list to free.
 */
void free_object_list(object_list_t *list) {
  for_all(list, free_object_list_elem);
  free(list->array);
  free(list);
}

/**
 * @brief Frees an object.
 *
 * Is a object_list_elem_function_t so can be called with for_all.
 * @param elem Object to free.
 */
void free_object_list_elem(object_list_elem_t *elem) {
  if (elem->ascii != NULL) {
    free(elem->ascii);
  }
  free(elem);
}
