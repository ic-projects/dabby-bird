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
#define WIDTH 400
#define HEIGHT 150
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
  pong_ball,
  pong_paddle_left,
  pong_paddle_right,
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


static char ball[] = "@";

static char paddle[] = "||||||||||||||||||||||||||||||||||||||||";

typedef void object_list_elem_function_t(object_list_elem_t *);


object_list_t *init_game(void);
void bounce(object_list_t *list);
void render_game(object_list_t *list, int y1, int y2);
int game_end(object_list_t *list);

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
  object_list_elem_t *object_b = (object_list_elem_t*) b;

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


/**
 * @brief Initailises a game state for a snake game.
 *
 * @returns An object list representing the initial game state.
 */
object_list_t *init_game(void) {
  srand(time(NULL));

  object_list_t *objects = new_list();
  vector_t zero = {0, 0};

  object_list_elem_t *paddle_left = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  paddle_left->point = (vector_t) {.x = 10, .y = HEIGHT/2};
  paddle_left->velocity = zero;
  paddle_left->acceleration = zero;
  paddle_left->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  paddle_left->ascii->height = 20;
  paddle_left->ascii->width = 2;
  paddle_left->ascii->ascii = paddle;
  paddle_left->ascii->color = 1;
  paddle_left->type = pong_paddle_left;
  paddle_left->depth = 0;
  add_elem(objects, paddle_left);

  object_list_elem_t *paddle_right = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  paddle_right->point = (vector_t) {.x = WIDTH - 10, .y = HEIGHT/2};
  paddle_right->velocity = zero;
  paddle_right->acceleration = zero;
  paddle_right->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  paddle_right->ascii->height = 20;
  paddle_right->ascii->width = 2;
  paddle_right->ascii->ascii = paddle;
  paddle_right->ascii->color = 1;
  paddle_right->type = pong_paddle_right;
  paddle_right->depth = 0;
  add_elem(objects, paddle_right);

  object_list_elem_t *ball_elem = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  ball_elem->point = (vector_t) {.x = WIDTH/2, .y = HEIGHT/2};
  ball_elem->velocity = (vector_t) {.x = 1, .y = 1};
  ball_elem->acceleration = zero;
  ball_elem->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  ball_elem->ascii->height = 1;
  ball_elem->ascii->width = 1;
  ball_elem->ascii->ascii = ball;
  ball_elem->ascii->color = 1;
  ball_elem->type = pong_ball;
  ball_elem->depth = 0;
  add_elem(objects, ball_elem);

  for_all(objects, print_object);

  cbreak();
  initscr();
  noecho();
  timeout(1);

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_RED);
  bkgd(COLOR_PAIR(3));

  return objects;
}

void bounce(object_list_t *list) {
  object_list_elem_t *elem = get_elem(list, pong_ball);
  if (elem->type == pong_ball) {
    if ((elem->point.y <= 0 && elem->velocity.y <= 0) || (elem->point.y >= HEIGHT && elem->velocity.y >= 0)) {
      elem->velocity.y *= -1;
    }
    if (is_covering(get_elem(list, pong_paddle_left), elem->point) && elem->velocity.x <= 0) {
      elem->velocity.x *= -2;
    }
    if (is_covering(get_elem(list, pong_paddle_right), elem->point) && elem->velocity.x >= 0) {
      elem->velocity.x *= -2;
    }
  }
}

int game_end(object_list_t *list) {
  object_list_elem_t *ball = get_elem(list, pong_ball);
  return ball->point.x <= 0 || ball->point.x >= WIDTH;
}

/**
 * @brief Renders the game, and updates the game state.
 *
 * @param list The object list.
 * @param dir Direction for the snake head to go.
 */
void render_game(object_list_t *list, int y1, int y2) {
  clear();
  bounce(list);
  get_elem(list, pong_paddle_left)->point.y = y1;
  for_all(list, move_object);
  printw("y1: %d\n", y1);
  get_elem(list, pong_paddle_right)->point.y = y2;
  print_game(list, WIDTH, HEIGHT);
  refresh();
}
