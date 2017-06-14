#include <stdlib.h>
#include <stdint.h>

typedef struct {
  uint32_t *array;
  uint8_t size;
} int_array_t;

uint32_t max(int_array_t *data);
uint32_t avg(int_array_t *data);
