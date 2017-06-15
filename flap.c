#include <stdbool.h>

void detect_flap(IplImage *frame, hands_t *hands, bool *is_down) {
  int half_y = frame->height / 2;
  if (*is_down && hands->left_y < half_y) {
    *is_down = false;
  } else if (!(*is_down) && hands->left_y > half_y) {
    *is_down = true;
    printf("Flap\n");
  }
}
