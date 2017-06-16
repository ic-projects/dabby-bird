#include "object_list.h"

int compare_list_elem( const void* a, const void* b) {
  object_list_elem_t *object_a = (object_list_elem_t*) a;
  object_list_elem_t *object_b = (object_list_elem_t*) b ;

  if (object_a->depth == object_b->depth) {
    return 0;
  } else if (object_a->depth < object_b->depth) {
    return -1;
  } else {
    return 1;
  }
}

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
//  qsort(list->array[0], list->size, sizeof(object_list_elem_t*), compare_list_elem);
}

int is_covering (object_list_elem_t *elem, vector_t point) {
  if ((elem->point.x <= point.x) && ((elem->point.x + elem->ascii->width) > point.x)) {
    if ((elem->point.y <= point.y) && ((elem->point.y + elem->ascii->height) > point.y)) {
      return 1;
    }
  }
  return 0;
}

char get_char_ascii(ascii_t *ascii, vector_t point) {
  return ascii->ascii[point.y * ascii->width + point.x];
}

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

void for_all(object_list_t *list, object_list_elem_function_t function) {
  for (int i = 0; i < list->size; i++) {
    function(list->array[i]);
  }
}

void move_object(object_list_elem_t *elem) {
  elem->point.x += elem->velocity.x;
  elem->point.y += elem->velocity.y;
  elem->velocity.x += elem->acceleration.x;
  elem->velocity.y += elem->acceleration.y;
}

void print_object(object_list_elem_t *elem) {
  printf("\nObject type: %d\n", elem->type);
  printf("Has:\n");
  printf("Depth: %d\n", elem->depth);
  printf("Position: (%d, %d)\n", elem->point.x, elem->point.y);
  //printf("Velocity: (%d, %d)\n", elem->velocity.x, elem->velocity.y);
  printf("Char at 0, 0: %c\n", get_char_ascii(elem->ascii, (vector_t){0, 0}));
}



object_list_elem_t *get_bird (object_list_t *list) {
  for (int i = 0; i < list->size; i++) {
    if (list->array[i]->type == bird) {
      return list->array[i];
    }
  }
  return NULL;
}

object_list_elem_t *get_head(object_list_t *list) {
  for (int i = 0; i < list->size; i++) {
    if (list->array[i]->type == snake_head) {
      return list->array[i];
    }
  }
  return NULL;
}

object_list_elem_t *get_tail(object_list_t *list) {
  for (int i = 0; i < list->size; i++) {
    if (list->array[i]->type == snake_tail) {
      return list->array[i];
    }
  }
  return NULL;
}

object_list_elem_t *get_apple(object_list_t *list) {
  for (int i = 0; i < list->size; i++) {
    if (list->array[i]->type == snake_apple) {
      return list->array[i];
    }
  }
  return NULL;
}

int get_color(object_list_t *list, vector_t point) {
  for (int i = 0; i < list->size; i++) {
    if (is_covering(list->array[i], point)) {
      return list->array[i]->ascii->color;
    }
  }
  return 1;
}

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

void free_object_list(object_list_t *list) {
  for_all(list, free_object_list_elem);
  free(list->array);
  free(list);
}

void free_object_list_elem(object_list_elem_t *elem) {
  free(elem->ascii);
  free(elem);
}
