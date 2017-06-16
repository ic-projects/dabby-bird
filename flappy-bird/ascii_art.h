/**
 * @file ascii_art.h
 * @brief Struct for storing ascii-art.
 */
#ifndef ascii_art_h
#define ascii_art_h
#include <stdint.h>
#define EMPTY_SPACE ' '

typedef struct {
  char *ascii;
  int color;
  uint16_t height;
  uint16_t width;
} ascii_t;

#endif
