typedef struct {
  int left_x;
  int left_y;
  int right_x;
  int right_y;
  bool is_null;
} hands_t;

hands_t *init_hands(void) {
  hands_t *h = (hands_t *) malloc(sizeof(hands_t));
  h->is_null = true;
  return h;
}

hands_t *clone_hands(hands_t *hands) {
  hands_t *h = init_hands();
  *h = *hands;
  return h;
}

double dist(int x, int y, int ux, int uy) {
  return sqrt((x - ux) * (x - ux) + (y - uy) * (y - uy));
}

void apply_force_point(IplImage *frame, int *px, int *py, double initial, double scale) {
  double new_x = initial;
  double new_y = 0;
  double pixel_weight;
  double dist_scale;
  int radius = 100;

  for (int y = *py - radius; y < *py + radius; y++) {
    for (int x = *px - radius; x < *px + radius; x++) {
      if (x > 0 && x < frame->width && y > 0 && y < frame->height) {
        pixel_weight = (double) ((unsigned char) (frame->imageData[y * frame->widthStep + x])) * scale;
        dist_scale = (double) (20) / ((double) (20 + dist(x, y, *px, *py)));
        new_x += pixel_weight * dist_scale * (x - *px);
        new_y += pixel_weight * dist_scale * (y - *py);
      }
    }
  }
  *px = *px + new_x;
  *py = *py + new_y;
}

void apply_force(IplImage *frame, hands_t *h, double scale) {
  int force = 1;
  apply_force_point(frame, &h->left_x, &h->left_y, -force, scale);
  apply_force_point(frame, &h->right_x, &h->right_y, force, scale);
}

bool outside_range(int x, int y, int rx, int ry, int rwidth, int rheight) {
  return !(x > rx && x < rwidth + rx && y > ry && y < rheight + ry);
}

// takes the frame and the last hands pos and updates hands to be the new pos of hands
void detect_hands(IplImage *frame, hands_t *hands) {
  if (hands == NULL || hands->is_null) {
    hands->left_x = 2 * frame->width / 7;
    hands->left_y = frame->height / 2;
    hands->right_x = 5 * frame->width / 7;
    hands->right_y = frame->height / 2;
    hands->is_null = false;
  }

  if (outside_range(hands->left_x, hands->left_y, 0, frame->height * 0.1, frame->width * 0.3, frame->height * 0.8)) {
    hands->left_x = 2 * frame->width / 7;
    hands->left_y = frame->height / 2;
  }

  if (outside_range(hands->right_x, hands->right_y, frame->width * 0.7 , frame->height * 0.1, frame->width * 0.3, frame->height * 0.8)) {
    hands->right_x = 5 * frame->width / 7;
    hands->right_y = frame->height / 2;
  }

  int ITERATIONS = 10;
  for (int i = 0; i < ITERATIONS; i++) {
    apply_force(frame, hands, 0.000005 * (ITERATIONS - i) / ITERATIONS);
  }
}
