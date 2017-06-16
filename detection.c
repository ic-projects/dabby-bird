#define ITERATIONS 10

/**
 * @brief A struct that holds information about the positions of hands.
 */
typedef struct {
  int left_x;
  int left_y;
  int right_x;
  int right_y;
  bool is_null;
} hands_t;

/**
 * @brief Initialises the hands_t struct.
 * @returns A pointer to the new hands_t struct.
 */
hands_t *init_hands(void) {
  hands_t *h = (hands_t *) malloc(sizeof(hands_t));
  h->is_null = true;
  return h;
}

/**
 * @brief Calculates distance between 2 points.
 * @param x x position of point1.
 * @param y y position of point1.
 * @param ux x position of point2.
 * @param uy y position of point2.
 * @returns The distance between the 2 points.
 */
double dist(int x, int y, int ux, int uy) {
  return sqrt((x - ux) * (x - ux) + (y - uy) * (y - uy));
}

/**
 * @brief Applies forces to a single point, converging it to the users hand.
 * @param frame The webcam image.
 * @param px A pointer to the x position of the point.
 * @param py A pointer to the y position of the point.
 * @param initial The initial x force to apply to the point.
 * @param scale Scaling for how much the point moves.
 */
void apply_force_point(IplImage *frame, int *px, int *py, double initial, double scale) {

  // Force accumulators.
  double new_x = initial;
  double new_y = 0;

  double pixel_weight;
  double dist_scale;
  int radius = 100;

  // Loop through all pixels around the points within radius distance.
  for (int y = *py - radius; y < *py + radius; y++) {
    for (int x = *px - radius; x < *px + radius; x++) {
      if (x > 0 && x < frame->width && y > 0 && y < frame->height) {
        // Weight based on pixel colour.
        pixel_weight = (double) ((unsigned char) (frame->imageData[y * frame->widthStep + x])) * scale;

        // Weight based on distance.
        dist_scale = (double) (20) / ((double) (20 + dist(x, y, *px, *py)));

        // Add force to accumulators.
        new_x += pixel_weight * dist_scale * (x - *px);
        new_y += pixel_weight * dist_scale * (y - *py);
      }
    }
  }

  // Update hands positions.
  *px = *px + new_x;
  *py = *py + new_y;
}

/**
 * @brief Applies forces to left and right hand points.
 * @param frame The webcam image.
 * @param h The hands struct to update.
 * @param scale Scaling for how much the point moves.
 */
void apply_force(IplImage *frame, hands_t *h, double scale) {
  int force = 1;

  // Apply force to left and right hand points.
  apply_force_point(frame, &h->left_x, &h->left_y, -force, scale);
  apply_force_point(frame, &h->right_x, &h->right_y, force, scale);
}

/**
 * @brief Calculates if a point is outside a region.
 * @param x X pos of the point.
 * @param y Y pos of the point.
 * @param rx X pos of the region.
 * @param ry Y pos of the region.
 * @param rwidth Width of the region.
 * @param rheight Height of the region.
 * @returns true if the point is outside the region, false otherwise.
 */
bool outside_range(int x, int y, int rx, int ry, int rwidth, int rheight) {
  return !(x > rx && x < rwidth + rx && y > ry && y < rheight + ry);
}

/**
 * @brief Detects new positions of the users hands.
 * @param frame The newest webcam frame.
 * @param hands The last position of the hands, is updated to be the new position.
 */
void detect_hands(IplImage *frame, hands_t *hands) {

  // Init hands to correct positions if it's the first frame.
  if (hands == NULL || hands->is_null) {
    hands->left_x = 2 * frame->width / 7;
    hands->left_y = frame->height / 2;
    hands->right_x = 5 * frame->width / 7;
    hands->right_y = frame->height / 2;
    hands->is_null = false;
  }

  // Reset hands position if the points drift off screen.
  if (outside_range(hands->left_x, hands->left_y, 0, frame->height * 0.1, frame->width * 0.3, frame->height * 0.8)) {
    hands->left_x = 2 * frame->width / 7;
    hands->left_y = frame->height / 2;
  }
  if (outside_range(hands->right_x, hands->right_y, frame->width * 0.7, frame->height * 0.1, frame->width * 0.3, frame->height * 0.8)) {
    hands->right_x = 5 * frame->width / 7;
    hands->right_y = frame->height / 2;
  }

  // Converge the points to the persons arms
  for (int i = 0; i < ITERATIONS; i++) {
    apply_force(frame, hands, 0.000005 * (ITERATIONS - i) / ITERATIONS);
  }
}
