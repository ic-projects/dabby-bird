#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <stdbool.h>

typedef struct {
  unsigned char h_max;
  unsigned char h_min;
  unsigned char s_max;
  unsigned char s_min;
  unsigned char v_max;
  unsigned char v_min;
  bool done;
  // int_array_t data;
} calibration_t;

bool in_box(int x, int y, int box_x, int box_y, int width);
void overlay_frame(IplImage *frame);
void calibrate_frame(IplImage *frame, calibration_t *c);
void generic_calibration(calibration_t *c);
void calibrate(CvCapture *capture, calibration_t *calibration);

#endif
