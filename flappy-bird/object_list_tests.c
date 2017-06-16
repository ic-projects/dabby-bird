#include "object_list.h"
#include <assert.h>

typedef void test_t(void);

void run_test(test_t test) {
  printf("Running tests: ");
  test();
  printf("Passed!\n");
}

void test_add(void) {
  printf("add\n");
  object_list_t *list = new_list();
  assert(list);
  assert(list->size == 0);
  assert(list->max_size == INITIAL_OBJECT_LIST_SIZE);
  object_list_elem_t *elem = malloc(sizeof(object_list_elem_t));
  elem->type = bird;
  add_elem(list, elem);
  assert(list->size == 1);
  assert(list->array[0]->type == bird);
  elem = malloc(sizeof(object_list_elem_t));
  elem->type = snake_head;
  add_elem(list, elem);
  assert(list->size == 2);
  assert(list->array[1]->type == snake_head);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 3);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 4);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 5);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 6);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 7);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 8);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 9);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 10);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 11);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 12);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 13);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 14);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 15);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 16);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 17);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 18);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 19);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 20);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 21);
  assert(list->max_size = 2 * INITIAL_OBJECT_LIST_SIZE);
  elem = malloc(sizeof(object_list_elem_t));
  add_elem(list, elem);
  assert(list->size == 22);
//  free_object_list(list);
}

void test_get_elem(void) {
  printf("get_elem\n");
  object_list_t *list = new_list();
  assert(list);
  assert(list->size == 0);
  object_list_elem_t *elem = malloc(sizeof(object_list_elem_t));
  elem->type = bird;
  add_elem(list, elem);
  assert(elem == get_elem(list, bird));
  elem = malloc(sizeof(object_list_elem_t));
  elem->type = snake_head;
  add_elem(list, elem);
  assert(elem == get_elem(list, snake_head));
  elem = malloc(sizeof(object_list_elem_t));
  elem->type = snake_tail;
  add_elem(list, elem);
  assert(elem == get_elem(list, snake_tail));
  elem = get_elem(list, bird);
  assert(elem->type == bird);
  elem = malloc(sizeof(object_list_elem_t));
  elem->type = snake_apple;
  add_elem(list, elem);
  assert(elem == get_elem(list, snake_apple));
  free_object_list(list);
}


int main(int argc, char **argv) {
  printf("Running tests:\n");
  run_test(test_add);
  run_test(test_get_elem);
  printf("All passed!\n");
  return EXIT_SUCCESS;
}
