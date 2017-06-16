bool in_range(unsigned char value, unsigned char min, unsigned char max, unsigned char range) {
  if (max >= min) {
    return value >= min && value <= max;
  } else {
    return value <= min && value >= 0 || value <= range && value >= max;
  }
}

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

double dist (int x1, int x2,
             int y1, int y2,
             int z1, int z2) {
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
}
