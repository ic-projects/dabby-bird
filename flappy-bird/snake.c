#include "snake.h"

static char snake[1] = "&";

static char char_apple[1] = "@";

object_list_t *init_game(void) {
  srand(time(NULL));

  object_list_t *objects = new_list();
  vector_t zero = {0, 0};

  object_list_elem_t *head = malloc(sizeof(object_list_elem_t));
  head->point = (vector_t){.x = WIDTH/2, .y = HEIGHT/2};
  head->velocity = zero;
  head->acceleration = zero;
  head->ascii = malloc(sizeof(ascii_t));
  head->ascii->height = 1;
  head->ascii->width = 1;
  head->ascii->ascii = snake;
  head->ascii->color = 3;
  head->type = snake_head;
  head->depth = 0;
  add_elem(objects, head);


  object_list_elem_t *body = malloc(sizeof(object_list_elem_t));
  body->point = (vector_t){.x = WIDTH/2 + 1, .y = HEIGHT/2};
  body->velocity = (vector_t){.x = -1};
  body->acceleration = zero;
  body->ascii = malloc(sizeof(ascii_t));
  body->ascii->height = 1;
  body->ascii->width = 1;
  body->ascii->ascii = snake;
  body->ascii->color = 1;
  body->type = snake_body;
  body->depth = 0;
  body->prev = head;
  add_elem(objects, body);

  object_list_elem_t *tail = malloc(sizeof(object_list_elem_t));
  tail->point = (vector_t){.x = WIDTH/2 + 2, .y = HEIGHT/2};
  tail->velocity = zero;
  tail->acceleration = zero;
  tail->ascii = malloc(sizeof(ascii_t));
  tail->ascii->height = 1;
  tail->ascii->width = 1;
  tail->ascii->ascii = snake;
  tail->ascii->color = 1;
  tail->type = snake_tail;
  tail->depth = 0;
  tail->prev = body;
  add_elem(objects, tail);


  object_list_elem_t *apple = malloc(sizeof(object_list_elem_t));
  apple->point = (vector_t){.x = rand() % WIDTH , .y = rand() % HEIGHT};
  apple->velocity = zero;
  apple->acceleration = zero;
  apple->ascii = malloc(sizeof(ascii_t));
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
  timeout(100);

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_RED, COLOR_BLUE);
  init_pair(3, COLOR_GREEN, COLOR_BLUE);
  bkgd(COLOR_PAIR(0));

  return objects;
}

void move_snake(object_list_t *list, vector_t dir) {
  object_list_elem_t *snake = get_elem(list, snake_tail);
  for (; snake->type != snake_head; snake = snake->prev) {
    snake->point = snake->prev->point;
  }
  snake->point = (vector_t){.x = snake->point.x + dir.x, .y = snake->point.y + dir.y};
  snake->point.x %= WIDTH;
  snake->point.y %= HEIGHT;
  if (snake->point.x < 0) {
    snake->point.x = WIDTH;
  }
  if (snake->point.y < 0) {
    snake->point.y = HEIGHT;
  }
}

void hit_apple(object_list_t *list) {
  object_list_elem_t *head_elem = get_elem(list, snake_head);
  object_list_elem_t *apple = get_elem(list, snake_apple);

  if (head_elem->point.x == apple->point.x && head_elem->point.y == apple->point.y) {
    object_list_elem_t *tail = get_elem(list, snake_tail);
    tail->type = snake_body;
    object_list_elem_t *ntail = malloc(sizeof(object_list_elem_t));
    ntail->point = tail->point;
    ntail->ascii = malloc(sizeof(ascii_t));
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

void render_game(object_list_t *list, vector_t dir) {
  clear();
  move_snake(list, dir);
  hit_apple(list);
  print_game(list, WIDTH, HEIGHT);
  refresh();
}

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
