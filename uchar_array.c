/**
 * @brief A struct to hold a unsigned char array.
 * Also holds the size, and the standard deviation.
 */
typedef struct {
  /** The unsigned char array. */
  unsigned char *array;
  /** The standard deviation of the values in the unsigned char array. */
  double standard_dev;
  /** The number of elements in the unsigned char array. */
  int size;
} uchar_array_t;

/**
 * @brief Finds the maximum value inside a given unsigned char array.
 * Note that if the array is empty then the maximum value is zero.
 * @param data The unsigned char array.
 * @returns The maximum value.
 */
unsigned char max(uchar_array_t *data) {
  unsigned char result = 0;
  for (int i = 0; i < data->size; i++) {
    if (data->array[i] > result) {
      result = data->array[i];
    }
  }
  return result;
}

/**
 * @brief Finds the average value of a given unsigned char array.
 * @param data The unsigned char array.
 * @returns The average value.
 */
double avg(uchar_array_t *data) {
  int total = 0;
  for (int i = 0; i < data->size; i++) {
    total += data->array[i];
  }
  return (total / data->size);
}

/**
 * @brief Sets the standard deviation of a given uchar_array_t struct.
 * @param data The uchar_array_t struct.
 */
void standard_dev(uchar_array_t *data) {
  double average = avg(data);
  double total = 0;
  printf("%i\n", data->size);

  for (int i = 0; i < data->size; i++) {
    total += pow(data->array[i] - average, 2);
  }

  total /= data->size;
  data->standard_dev = sqrt(total);
}

/**
 * @brief Finds the lower quartile of a given unsigned char array.
 * @param data The unsigned char array.
 * @param dev The deviation.
 * @returns The lower quartile.
 */
unsigned char lower(uchar_array_t *data, double dev) {
  return avg(data) - (dev * data->standard_dev);
}

/**
 * @brief Finds the upper quartile of a given unsigned char array.
 * @param data The unsigned char array.
 * @param dev The deviation.
 * @returns The upper quartile.
 */
unsigned char upper(uchar_array_t *data, double dev) {
  return avg(data) + (dev * data->standard_dev);
}

/**
 * @brief Initialises a uchar_array_t struct.
 * @param size The number of elements in the array.
 * @returns The pointer to the initialised struct.
 */
uchar_array_t *init_arr(int size) {
  uchar_array_t *arr = (uchar_array_t *) malloc(sizeof(uchar_array_t));
  arr->size = size;
  arr->array = (unsigned char *) malloc(sizeof(unsigned char) * size);
  return arr;
}
