#include <stdbool.h>

typedef struct {
  int h_max;
  int h_min;
  int s_max;
  int s_min;
  int v_max;
  int v_min;
  bool done;
  //list_t data;
} calibration_t;

bool in_box(int x, int y, int box_x, int box_y, int width) {
  return (x > box_x - width
    && x < box_x + width
    && y > box_y - width
    && y < box_y + width);
}

void overlay_frame(IplImage *frame) {
  for (int y = 0; y < frame->height; y++) {
    for (int x = 0; x < frame->width; x++) {
      if (!in_box(x, y, frame->width/2, frame->height/2, frame->width/8)) {
        for (int u = 0; u < frame->nChannels; u++) {
          frame->imageData[y * frame->widthStep + x * frame->nChannels + u] = (unsigned char) (frame->imageData[y * frame->widthStep + x * frame->nChannels + u]) / 2;
        }
      }
    }
  }
}

void calibrate_frame(IplImage *frame, calibration_t *calibration) {
  // ADD TO LIST
  // REMOVE ELEMENT OVER 64
  // IF LIST IS FULL
  //  CALCULATE AVERAGE
  //  IF NEWEST DIFF TO AVERAGE
}

void generic_calibration(calibration_t *c) {
  c->h_max = 0;
  c->h_min = 18;
  c->s_max = 10;
  c->s_min = 150;
  c->v_max = 60;
  c->v_min = 255;
  c->done = true;
}

void calibrate(CvCapture *capture, calibration_t *calibration) {
  IplImage *frame = 0;
  calibration->done = false;

  while (cvWaitKey(10) != 'q' || calibration->done) {
    frame = cvQueryFrame(capture);

    if (frame) {
      overlay_frame(frame);
      cvCvtColor(frame, frame, CV_BGR2HSV);
      calibrate_frame(frame, calibration);
      cvCvtColor(frame, frame, CV_HSV2BGR);
      cvShowImage("Calibrate", frame);
    }
  }

  cvDestroyWindow("Calibrate");
  generic_calibration(calibration);
}
