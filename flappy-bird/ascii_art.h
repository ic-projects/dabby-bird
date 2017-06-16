/**
 * @file ascii_art.h
 * @brief Header to define the ascii_t struct.
 */

#ifndef ascii_art_h
#define ascii_art_h
#include <stdint.h>

/** Defines a blank character for use in ASCII output. */
#define EMPTY_SPACE ' '

/**
 * @brief A struct to hold information about an ASCII sprite.
 */
typedef struct {
  /** The ASCII string used to represent the object. */
  char *ascii;
  /**
   * The color of the ASCII object. This number refers to a foreground /
   * background colour pair, which must be defined at the same time as object
   * definition. This is done using start_color and init_pair. See the Linux
   * man page for more information.
   */
  int color;
  /** The height, in characters, of the ASCII object. */
  uint16_t height;
  /** The width, in characters, of the ASCII object. */
  uint16_t width;
} ascii_t;

#endif
