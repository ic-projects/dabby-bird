//#include "int_array.h"
#include <math.h>

typedef struct {
  unsigned char *array;
  double standard_dev;
  int size;
} int_array_t;

unsigned char max(int_array_t *data) {
  unsigned char result = 0;
  for (uint8_t i = 0; i < data->size; i++) {
    if (data->array[i] > result) {
      result = data->array[i];
    }
  }
  return result;
}

double avg(int_array_t *data) {
  uint32_t total = 0;
  for (int i = 0; i < data->size; i++) {
    total += data->array[i];
  }
  return (total / data->size);
}

void standard_dev(int_array_t *data) {
  double average = avg(data);
  double total = 0;
  printf("%i\n", data->size);
  for (int i = 0; i < data->size; i++) {
    total += pow(data->array[i] - average, 2);
  }
  total /= data->size;
  data->standard_dev = sqrt(total);
}

unsigned char lower(int_array_t *data, double dev) {
  return avg(data) - (dev * data->standard_dev);
}

unsigned char upper(int_array_t *data, double dev) {
  return avg(data) + (dev * data->standard_dev);
}

int_array_t *init_arr(int size) {
  int_array_t *arr = (int_array_t *) malloc(sizeof(int_array_t));
  arr->size = size;
  arr->array = (unsigned char *) malloc(sizeof(unsigned char) * size);
  return arr;
}
