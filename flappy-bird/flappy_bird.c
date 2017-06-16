#include "flappy_bird.h"

static char pipe_ascii[600] = "|#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#|                                                                              |#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#||#|";

static char grass_ascii[1200] = "/////\\\\//\\////\\\\///\\\\////////\\\\/||||////\\\\/////////\\\\\\\\)))))\\/////\\\\\\\\/////////\\((((\\\\\\////////\\\\\\))))))\\///|||||||////\\\\\\((((\\\\\\///\\\\///\\\\\\\\/////\\\\\\///////\\\\////\\\\\\////\\\\\\\\///\\\\//\\//\\///\\/\\/\\\\\\\\\\\\||||/////|||\\\\\\\\\\))))))(((////\\\\/////\\\\//\\////\\\\///\\\\////////\\\\/||||////\\\\/////////\\\\\\\\)))))\\/////\\\\\\\\/////////\\((((\\\\\\////////\\\\\\))))))\\///|||||||////\\\\\\((((\\\\\\///\\\\///\\\\\\\\/////\\\\\\///////\\\\////\\\\\\////\\\\\\\\///\\\\//\\//\\///\\/\\/\\\\\\\\\\\\||||/////|||\\\\\\\\\\))))))(((////\\\\                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ";

static char bird_ascii[8] = "(@@)\"||\"";

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

  object_list_elem_t *elem2 = malloc(sizeof(object_list_elem_t));
  elem2->point = (vector_t) {.x = 0, .y = (HEIGHT - 2)};
  elem2->velocity = (vector_t) {.x = -1};
  elem2->acceleration = zero;
  elem2->ascii = malloc(sizeof(ascii_t));
  elem2->ascii->height = 2;
  elem2->ascii->width = 600;
  elem2->ascii->ascii = grass_ascii;
  elem2->ascii->color = 3;
  elem2->type = ground;
  elem2->depth = 3;
  add_elem(objects, elem2);

  object_list_elem_t *elem1 = malloc(sizeof(object_list_elem_t));
  elem1->point = (vector_t) {.x = 10, .y = HEIGHT/2};
  elem1->velocity = zero;
  elem1->acceleration = (vector_t) {.y = 1};
  elem1->ascii = malloc(sizeof(ascii_t));
  elem1->ascii->height = 2;
  elem1->ascii->width = 4;
  elem1->ascii->ascii = bird_ascii;
  elem1->ascii->color = 1;
  elem1->type = bird;
  elem1->depth = 1;
  add_elem(objects, elem1);

  object_list_elem_t *elem = malloc(sizeof(object_list_elem_t));
  elem->velocity = (vector_t) {.x = -5};
  elem->acceleration = zero;
  elem->ascii = malloc(sizeof(ascii_t));
  elem->ascii->height = 155;
  elem->ascii->width = 3;
  elem->point = (vector_t) {.x = WIDTH - 10, .y =  -(rand() % HEIGHT/2) + HEIGHT/4 + (HEIGHT/2 - (elem->ascii->height / 2))};
  elem->ascii->ascii = pipe_ascii;
  elem->ascii->color = 1;
  elem->type = pipes;
  elem->depth = 2;
  add_elem(objects, elem);

  object_list_elem_t *elem4 = malloc(sizeof(object_list_elem_t));
  elem4->velocity = (vector_t) {.x = -5};
  elem4->acceleration = zero;
  elem4->ascii = malloc(sizeof(ascii_t));
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
