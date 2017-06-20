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
#define WIDTH 50
#define HEIGHT 50
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


static char snake[] = "&";

static char char_apple[] = "@";

typedef void object_list_elem_function_t(object_list_elem_t *);


int snake_hit(object_list_t *list);
object_list_t *init_game(void);
void move_snake(object_list_t *list, vector_t dir);
void render_game(object_list_t *list, vector_t dir);
void hit_apple(object_list_t *list);

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


/**
 * @brief Initailises a game state for a snake game.
 *
 * @returns An object list representing the initial game state.
 */
object_list_t *init_game(void) {
  srand(time(NULL));

  object_list_t *objects = new_list();
  vector_t zero = {0, 0};

  object_list_elem_t *head = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  head->point = (vector_t) {.x = WIDTH/2, .y = HEIGHT/2};
  head->velocity = zero;
  head->acceleration = zero;
  head->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  head->ascii->height = 1;
  head->ascii->width = 1;
  head->ascii->ascii = snake;
  head->ascii->color = 3;
  head->type = snake_head;
  head->depth = 0;
  add_elem(objects, head);


  object_list_elem_t *body = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  body->point = (vector_t) {.x = WIDTH/2 + 1, .y = HEIGHT/2};
  body->velocity = (vector_t) {.x = -1};
  body->acceleration = zero;
  body->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  body->ascii->height = 1;
  body->ascii->width = 1;
  body->ascii->ascii = snake;
  body->ascii->color = 1;
  body->type = snake_body;
  body->depth = 0;
  body->prev = head;
  add_elem(objects, body);

  object_list_elem_t *tail = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  tail->point = (vector_t) {.x = WIDTH/2 + 2, .y = HEIGHT/2};
  tail->velocity = zero;
  tail->acceleration = zero;
  tail->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  tail->ascii->height = 1;
  tail->ascii->width = 1;
  tail->ascii->ascii = snake;
  tail->ascii->color = 1;
  tail->type = snake_tail;
  tail->depth = 0;
  tail->prev = body;
  add_elem(objects, tail);


  object_list_elem_t *apple = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
  apple->point = (vector_t) {.x = rand() % WIDTH, .y = rand() % HEIGHT};
  apple->velocity = zero;
  apple->acceleration = zero;
  apple->ascii = (ascii_t *) malloc(sizeof(ascii_t));
  apple->ascii->height = 1;
  apple->ascii->width = 1;
  apple->ascii->ascii = char_apple;
  apple->ascii->color = 2;
  apple->type = snake_apple;
  apple->depth = 1;
  add_elem(objects, apple);

  for_all(objects, print_object);

  cbreak();
  initscr();
  noecho();
  timeout(1);

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_RED, COLOR_BLUE);
  init_pair(3, COLOR_GREEN, COLOR_BLUE);
  bkgd(COLOR_PAIR(0));

  return objects;
}

/**
 * @brief Moves the snake.
 *
 * @param list The object list.
 * @param dir Direction for the snake head to go.
 */
void move_snake(object_list_t *list, vector_t dir) {
  object_list_elem_t *snake = get_elem(list, snake_tail);
  for (; snake->type != snake_head; snake = snake->prev) {
    snake->point = snake->prev->point;
  }
  snake->point = (vector_t) {.x = snake->point.x + dir.x, .y = snake->point.y + dir.y};
  snake->point.x %= WIDTH;
  snake->point.y %= HEIGHT;
  if (snake->point.x < 0) {
    snake->point.x = WIDTH;
  }
  if (snake->point.y < 0) {
    snake->point.y = HEIGHT;
  }
}

/**
 * @brief Checks if apple is hit, creates new apple if it is.
 *
 * It also creates lengthens the snake if an apple is hit.
 * @param list The object list.
 */
void hit_apple(object_list_t *list) {
  object_list_elem_t *head_elem = get_elem(list, snake_head);
  object_list_elem_t *apple = get_elem(list, snake_apple);

  if (head_elem->point.x == apple->point.x && head_elem->point.y == apple->point.y) {
    object_list_elem_t *tail = get_elem(list, snake_tail);
    tail->type = snake_body;
    object_list_elem_t *ntail = (object_list_elem_t *) malloc(sizeof(object_list_elem_t));
    ntail->point = tail->point;
    ntail->ascii = (ascii_t *) malloc(sizeof(ascii_t));
    ntail->ascii->height = 1;
    ntail->ascii->width = 1;
    ntail->ascii->ascii = snake;
    ntail->ascii->color = 1;
    ntail->type = snake_tail;
    ntail->depth = 0;
    ntail->prev = tail;
    add_elem(list, ntail);

    apple->point.y = rand() % HEIGHT;
    apple->point.x = rand() % WIDTH;
  }

}

/**
 * @brief Renders the game, and updates the game state.
 *
 * @param list The object list.
 * @param dir Direction for the snake head to go.
 */
void render_game(object_list_t *list, vector_t dir) {
  clear();
  move_snake(list, dir);
  hit_apple(list);
  print_game(list, WIDTH, HEIGHT);
  refresh();
}

/**
 * @brief Checks if the snake has hit itself
 *
 * @param list The object list.
 */
int snake_hit(object_list_t *list) {
  object_list_elem_t *head = get_elem(list, snake_head);
  for (int i = 0; i < list->size; i++) {
    if (list->array[i]->type == snake_body || list->array[i]->type == snake_tail) {
      if (list->array[i]->point.x == head->point.x && list->array[i]->point.y == head->point.y) {
        return 1;
      }
    }
  }
  return 0;
}
