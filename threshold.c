// #include "threshold.h"
#include <math.h>

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

double dist (int x1, int x2,
             int y1, int y2,
             int z1, int z2) {
  return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2) + pow(z1-z2, 2));
}


IplImage *get_arm_variable(IplImage *frame, calibration_t *c) {
  IplImage *result = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    for (int y = 0; y < frame->height; y++) {
    for (int x = 0; x < frame->width; x++) {
        if (fabs((int)(unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels] - (int) c->h_max) <= 40  &&
            fabs((int)(unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels + 1] - (int) c->s_max) <= 150 &&
            fabs((int)(unsigned char) frame->imageData[y * frame->widthStep + x * frame->nChannels + 2] - (int) c->v_max) <= 210) {
          result->imageData[y * result->widthStep + x] = 255;
        } else {
          result->imageData[y * result->widthStep + x] = 0;
        }
    }
  }

  return result;
}
