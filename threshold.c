/**
 * @file threshold.c
 * @brief Functions to determine skin-coloured pixel within a frame.
 */

/**
 * @brief Checks whether a given value is in a given range.
 * Note that the minimum and maximum values can sometimes be swapped due to the
 * fact that HSV is circular.
 * @param value The value to check whether it is in range.
 * @param min Normally the minimum value to accept.
 * @param max Normally the maximum value to accept.
 * @param range The maximum value that is allowed.
 * @returns True iff the value is in the specified range.
 */
bool in_range(unsigned char value, unsigned char min, unsigned char max, unsigned char range) {
  if (max >= min) {
    return value >= min && value <= max;
  } else {
    return value <= min && value >= 0 || value <= range && value >= max;
  }
}

/**
 * @brief Gets a black and white frame of where the skin is.
 * Given a frame and a calibration struct, it marks each pixel white where skin
 * is present.
 * @param frame The IplImage frame.
 * @param c The calibration which contains the skin colour.
 * @returns A black and white IplImage frame indicating where skin is.
 */
IplImage *get_arm(IplImage *frame, calibration_t *c) {
  IplImage *result = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);

  for (int y = 0; y < frame->height; y++) {
    for (int x = 0; x < frame->width; x++) {
      if (in_range((unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels], c->h_min, c->h_max, 180) &&
          in_range((unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels + 1], c->s_min, c->s_max, 255) &&
          in_range((unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels + 2], c->v_min, c->v_max, 255)) {
        result->imageData[y * result->widthStep + x] = 255;
      } else {
        result->imageData[y * result->widthStep + x] = 0;
      }
    }
  }

  return result;
}

/**
 * @brief Finds distance between 2 points in 3D space.
 * @param x1 The x coordinate of the first point.
 * @param x2 The x coordinate of the second point.
 * @param y1 The y coordinate of the first point.
 * @param y2 The y coordinate of the second point.
 * @param z1 The z coordinate of the first point.
 * @param z2 The z coordinate of the second point.
 * @returns The distance between the 2 point.
 */
double dist(int x1, int x2,
            int y1, int y2,
            int z1, int z2) {
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
}
