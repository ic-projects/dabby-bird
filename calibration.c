// #include "calibration.h"
#include "int_array.c"



typedef struct {
  unsigned char h_max;
  unsigned char h_min;
  unsigned char s_max;
  unsigned char s_min;
  unsigned char v_max;
  unsigned char v_min;
  bool done;
  CvScalar upper;
  CvScalar lower;
  // int_array_t data;
} calibration_t;

bool in_box(int x, int y, int box_x, int box_y, int width, int height) {
  return (x > box_x - width
    && x < box_x + width
    && y > box_y - height
    && y < box_y + height);
}

void overlay_frame(IplImage *frame, int reg_x, int reg_y, int reg_height, int reg_width) {
  for (int y = 0; y < frame->height; y++) {
    for (int x = 0; x < frame->width; x++) {
      if (!in_box(x, y, reg_x, reg_y, reg_width, reg_height)) {
        for (int w = 0; w < frame->nChannels; w++) {
          frame->imageData[y * frame->widthStep + x * frame->nChannels + w] = (unsigned char) (frame->imageData[y * frame->widthStep + x * frame->nChannels + w]) / 4;
        }
      }
    }
  }
}

void calibrate_frame(IplImage *frame, calibration_t *c) {
  // ADD TO LIST
  // REMOVE ELEMENT OVER 64
  // IF LIST IS FULL
  //  CALCULATE AVERAGE
  //  IF NEWEST DIFF TO AVERAGE
}

void final_calibration(IplImage *frame, calibration_t *c, int reg_x, int reg_y, int reg_height, int reg_width) {
  int size = 0;
  for (int y = reg_y - reg_height; y < reg_y + reg_height; y++) {
    for (int x = reg_x - reg_width; x < reg_x + reg_width; x++) {
      size++;
    }
  }
  int_array_t *h_arr = init_arr(size);
  int_array_t *s_arr = init_arr(size);
  int_array_t *v_arr = init_arr(size);
  int i = 0;
  for (int y = reg_y - reg_height; y < reg_y + reg_height; y++) {
    for (int x = reg_x - reg_width; x < reg_x + reg_width; x++) {
      h_arr->array[i] = frame->imageData[y * frame->widthStep + x * frame->nChannels];
      s_arr->array[i] = frame->imageData[y * frame->widthStep + x * frame->nChannels + 1];
      v_arr->array[i] = frame->imageData[y * frame->widthStep + x * frame->nChannels + 2];
      i++;
    }
  }
  standard_dev(h_arr);
  standard_dev(s_arr);
  standard_dev(v_arr);
  printf("h dev: %f\n", (float) (h_arr->standard_dev));
  c->done = true;
  /*c->h_max = upper(h_arr, 1);
  c->h_min = lower(h_arr, 1);
  c->s_max = upper(s_arr, 3);
  c->s_min = lower(s_arr, 3);
  c->v_max = upper(v_arr, 3);
  c->v_min = lower(v_arr, 3);*/
  double range = 0.4;
  c->h_max = avg(h_arr);
  c->h_min = c->h_max * (1 - range);
  c->h_max *= 1 + range;
  c->s_max = avg(s_arr);
  c->s_min = c->s_max * (1 - range);
  c->s_max *= 1 + range;
  c->v_max = avg(v_arr);
  c->v_min = c->v_max * (1 - range);
  c->v_max *= 1 + range;
  c->lower = cvScalar(c->h_min, c->s_min, c->v_min);
  c->upper = cvScalar(c->h_max, c->s_max, c->v_max);
  printf("h_max: %f\n", (float) (c->h_max));
  printf("h_min: %f\n", (float) (c->h_min));
  printf("s_max: %f\n", (float) (c->s_max));
  printf("s_min: %f\n", (float) (c->s_min));
  printf("v_max: %f\n", (float) (c->v_max));
  printf("v_min: %f\n", (float) (c->v_min));
}

void generic_calibration(calibration_t *c) {
  c->h_max = 25;
  c->h_min = 0;
  c->s_max = 150;
  c->s_min = 10;
  c->v_max = 255;
  c->v_min = 60;
  c->done = true;
}

void calibrate(CvCapture *capture, calibration_t *calibration) {
  IplImage *frame = 0;
  calibration->done = false;
  int reg_x = 0;
  int reg_y = 0;
  int reg_height = 0;
  int reg_width = 0;
  int timer = 0;
  while ((cvWaitKey(10) != 'q' || calibration->done) && timer < 90) {
    frame = cvQueryFrame(capture);

    if (frame) {
      if(reg_x == 0) {
        reg_x = frame->width/2;
        reg_y = frame->height/2;
        reg_height = frame->height/20;
        reg_width = frame->width/20;
      }
      overlay_frame(frame,reg_x, reg_y, reg_height, reg_width);
      cvCvtColor(frame, frame, CV_BGR2HSV);
      calibrate_frame(frame, calibration);
      cvCvtColor(frame, frame, CV_HSV2BGR);
      cvFlip(frame,frame, 1);
      cvShowImage("Calibrate", frame);
    }
    timer++;
  }
  cvCvtColor(frame, frame, CV_BGR2HSV);
    printf("%i %i %i %i\n",reg_x,reg_y,reg_height,reg_width);
  final_calibration(frame, calibration, reg_x, reg_y, reg_height, reg_width);
  cvCvtColor(frame, frame, CV_HSV2BGR);

  cvDestroyWindow("Calibrate");
  //generic_calibration(calibration);
}
