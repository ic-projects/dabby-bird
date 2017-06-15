// #include "threshold.h"

IplImage *get_arm(IplImage *frame, calibration_t *c) {
  IplImage *result = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);

  for (int y = 0; y < frame->height; y++) {
    for (int x = 0; x < frame->width; x++) {
      if ((unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels] >= c->h_min &&
          (unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels] <= c->h_max &&
          (unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels + 1] >= c->s_min &&
          (unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels + 1] <= c->s_max &&
          (unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels + 2] >= c->v_min &&
          (unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels + 2] <= c->v_max) {
        result->imageData[y * result->widthStep + x] = 255;
      } else {
        result->imageData[y * result->widthStep + x] = 0;
      }
    }
  }

  return result;
}
