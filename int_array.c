#include "int_array.h"

uint32_t max(int_array_t *data) {
  uint32_t result = 0;
  for (uint8_t i = 0; i < data->size; i++) {
    if (data->array[i] > result) {
      result = data->array[i];
    }
  }
  return result;
}

uint32_t avg(int_array_t *data) {
  uint32_t total = 0;
  for (uint8_t i = 0; i < data->size; i++) {
    total += data->array[i];
  }
  return (total / data->size);
}
