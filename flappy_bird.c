#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define EMPTY_SPACE ' '
#define WIDTH 300
#define HEIGHT 100
#define INITIAL_OBJECT_LIST_SIZE 20

typedef struct {
  char *ascii;
  int color;
  uint16_t height;
  uint16_t width;
} ascii_t;

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

typedef struct object_list_elem object_list_elem_t;

struct object_list_elem {
  type_t type;
  vector_t point;
  vector_t velocity;
  vector_t acceleration;
  ascii_t *ascii;
  uint16_t depth;
  struct object_list_elem *prev;
};


typedef struct {
  object_list_elem_t **array;
  uint16_t size;
  uint16_t max_size;
} object_list_t;

static char pipe_ascii[] = "|#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#|                                                                              |#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#|";

static char grass_ascii[] = "/////\\\\//\\////\\\\///\\\\////////\\\\/||||////\\\\/////////\\\\\\\\)))))\\/////\\\\\\\\/////////\\((((\\\\\\////////\\\\\\))))))\\///|||||||////\\\\\\((((\\\\\\///\\\\///\\\\\\\\/////\\\\\\///////\\\\////\\\\\\////\\\\\\\\///\\\\//\\//\\///\\/\\/\\\\\\\\\\\\||||/////|||\\\\\\\\\\))))))(((////\\\\/////\\\\//\\////\\\\///\\\\////////\\\\/||||////\\\\/////////\\\\\\\\)))))\\/////\\\\\\\\/////////\\((((\\\\\\////////\\\\\\))))))\\///|||||||////\\\\\\((((\\\\\\///\\\\///\\\\\\\\/////\\\\\\///////\\\\////\\\\\\////\\\\\\\\///\\\\//\\//\\///\\/\\/\\\\\\\\\\\\||||/////|||\\\\\\\\\\))))))(((////\\\\                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ";

static char bird_ascii[] = "(@@)\"||\"";

typedef void object_list_elem_function_t(object_list_elem_t *);

void move_pipes(object_list_elem_t *elem);
void flap(object_list_elem_t *elem);
int bird_coll(object_list_t *list);
object_list_t *init_game(void);
void render_game(object_list_t *list);

object_list_t *new_list(void);
void remove_elem(object_list_t *list, uint16_t id);
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

int compare_list_elem(const void *a, const void *b) {
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
  object_list_t *list = (object_list_t *) malloc(sizeof(object_list_t));
  if (!list) {
    perror("Unable to allocate memory for new list");
    exit(EXIT_FAILURE);
  }
  list->array = (object_list_elem_t **) malloc(sizeof(object_list_elem_t *) * INITIAL_OBJECT_LIST_SIZE);
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
    list->array = (object_list_elem_t **) realloc(list->array, sizeof(object_list_elem_t *) * list->max_size);
    if (!list->array) {
      perror("Unable to reallocate memory for object list");
      exit(EXIT_FAILURE);
    }
  }
  list->array[list->size] = elem;
  list->size++;
  // qsort(list->array[0], list->size, sizeof(object_list_elem_t*), compare_list_elem);
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
  // printf("Velocity: (%d, %d)\n", elem->velocity.x, elem->velocity.y);
  printf("Char at 0, 0: %c\n", get_char_ascii(elem->ascii, (vector_t) {0, 0}));
}

object_list_elem_t *get_elem(object_list_t *list, type_t type) {
  for (int i = 0; i < list->size; i++) {
    if (list->array[i]->type == type) {
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
  if (elem->ascii != NULL) {
    free(elem->ascii);
  }
  free(elem);
}

void move_pipes(object_list_elem_t *elem) {
  if (elem->type == pipes) {
    if (elem->point.x <= -2) {
      elem->point.x = WIDTH;
      //elem->point.x += rand() % 100;
      elem->point.y = -(rand() % HEIGHT/2) + HEIGHT/4 + (HEIGHT/2 - (elem->ascii->height / 2));
    }
  }
  if (elem->type == ground) {
    if (elem->point.x <= -WIDTH/2) {
      elem->point.x = 0;
    }
  }
}

void flap(object_list_elem_t *elem) {
  if (elem->type == bird) {
    elem->velocity.y = -5;
  }
}


int bird_coll(object_list_t *list) {
  object_list_elem_t *bird_elem = get_elem(list, bird);
  for (int i = 0; i < list->size; i++) {
    if (list->array[i]->type == pipes) {
      vector_t test_point;
      if (is_covering(list->array[i], bird_elem->point)) {
        test_point = (vector_t) {.x = 0, .y = bird_elem->point.y - list->array[i]->point.y};
        return get_char_ascii(list->array[i]->ascii, test_point) != ' ';
      }
      test_point = (vector_t) {.x = bird_elem->point.x, .y = bird_elem->point.y + bird_elem->ascii->width};
      if (is_covering(list->array[i], test_point)) {
        test_point = (vector_t) {.x = 0, .y = bird_elem->point.y + bird_elem->ascii->width - list->array[i]->point.y};
        return get_char_ascii(list->array[i]->ascii, test_point) != ' ';
      }
    }
  }
  return bird_elem->point.y >= HEIGHT;
}

object_list_t *init_game(void) {
  srand(time(NULL));

  object_list_t *objects = new_list();
  vector_t zero = {0, 0};

  object_list_elem_t *elem2 = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  elem2->point = (vector_t) {.x = 0, .y = (HEIGHT - 2)};
  elem2->velocity = (vector_t) {.x = -1, .y = 0};
  elem2->acceleration = zero;
  elem2->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  elem2->ascii->height = 2;
  elem2->ascii->width = 600;
  elem2->ascii->ascii = grass_ascii;
  elem2->ascii->color = 3;
  elem2->type = ground;
  elem2->depth = 3;
  add_elem(objects, elem2);

  object_list_elem_t *elem1 = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  elem1->point = (vector_t) {.x = 10, .y = HEIGHT/2};
  elem1->velocity = zero;
  elem1->acceleration = (vector_t) {.x = 0, .y = 1};
  elem1->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  elem1->ascii->height = 2;
  elem1->ascii->width = 4;
  elem1->ascii->ascii = bird_ascii;
  elem1->ascii->color = 1;
  elem1->type = bird;
  elem1->depth = 1;
  add_elem(objects, elem1);

  object_list_elem_t *elem = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  elem->velocity = (vector_t) {.x = -5};
  elem->acceleration = zero;
  elem->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  elem->ascii->height = 155;
  elem->ascii->width = 3;
  elem->point = (vector_t) {.x = WIDTH - 10, .y =  -(rand() % HEIGHT/2) + HEIGHT/4 + (HEIGHT/2 - (elem->ascii->height / 2))};
  elem->ascii->ascii = pipe_ascii;
  elem->ascii->color = 1;
  elem->type = pipes;
  elem->depth = 2;
  add_elem(objects, elem);

  object_list_elem_t *elem4 = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  elem4->velocity = (vector_t) {.x = -5};
  elem4->acceleration = zero;
  elem4->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  elem4->ascii->height = 155;
  elem4->ascii->width = 3;
  elem4->point = (vector_t) {.x = WIDTH * 1.5 - 10, .y =  -(rand() % HEIGHT/2) + HEIGHT/4 + (HEIGHT/2 - (elem->ascii->height / 2))};
  elem4->ascii->ascii = pipe_ascii;
  elem4->ascii->color = 1;
  elem4->type = pipes;
  elem4->depth = 2;
  add_elem(objects, elem4);

  for_all(objects, print_object);

  cbreak();
  initscr();
  noecho();
  timeout(50);
  setlocale(LC_ALL, "");

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_BLUE, COLOR_BLUE);
  init_pair(3, COLOR_GREEN, COLOR_BLUE);
  init_pair(6, COLOR_GREEN, COLOR_GREEN);
  bkgd(COLOR_PAIR(0));

  return objects;

}

void render_game(object_list_t *list) {
  clear();
  for_all(list, move_object);
  for_all(list, move_pipes);
  print_game(list, WIDTH, HEIGHT);
  refresh();
}
