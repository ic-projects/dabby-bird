#include "cv.h"
#include "highgui.h"
#include "calibration.c"
#include "threshold.c"
#include "detection.c"
#include "median_blur.c"
#include "flap.c"

void generate_movement_frame(IplImage *debug_frame, const IplImage *prev_frame, const IplImage *frame);

int main(int argc, char **argv) {
  CvCapture *capture = 0;
  IplImage *frame = 0;
  IplImage *prev_frame = 0;
  IplImage *result = 0;
  IplImage *arm = 0;
  IplImage *blurred_arm = 0;
  hands_t *hands = init_hands();
  CvScalar red = cvScalar(255, 0, 0);
  capture = cvCaptureFromCAM(0);
  bool is_down = false;

  if(!capture) {
    perror("Error when reading steam");
    exit(EXIT_FAILURE);
  }

  // cvNamedWindow("Calibrate", 1);
  // calibration_t *cali = (calibration_t *) malloc(sizeof(calibration_t));
  // calibrate(capture, cali);

  calibration_t *c = (calibration_t *) malloc(sizeof(calibration_t));
  calibrate(capture, c);
  // generic_calibration(c);


  cvNamedWindow("Dabby Bird", 1);
  cvNamedWindow("Arm Detection", 1);

  while (cvWaitKey(10) != 'q') {
    frame = cvQueryFrame(capture);

    if (frame) {
      if (prev_frame) {
        prev_frame = cvCloneImage(frame);

        cvCvtColor(frame, frame, CV_BGR2HSV);
        arm = get_arm(frame, c);
        cvCircle(frame, cvPoint(10,10), 10, c->lower, 15);
        cvCircle(frame, cvPoint(40,10), 10, c->upper, 15);
        cvCvtColor(frame, frame, CV_HSV2BGR);
        detect_hands(arm, hands);
        detect_flap(frame, hands, &is_down);

        cvCircle(frame, cvPoint(hands->left_x, hands->left_y), 10, red, 15);
        cvCircle(frame, cvPoint(hands->right_x, hands->right_y), 10, red, 15);

        cvShowImage("Arm Detection", arm);
      } else {
        arm = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
        blurred_arm = cvCreateImage(get_blurred_size(cvGetSize(frame)), IPL_DEPTH_8U, 1);
      }
      prev_frame = cvCloneImage(frame);
      cvFlip(frame, frame, 1);
      cvShowImage("Dabby Bird", frame);
    }
  }

  cvDestroyWindow("Dabby Bird");
  cvDestroyWindow("Arm Detection");
  cvReleaseCapture(&capture);

  // free(c);
  // cvReleaseCapture(&capture);

  return EXIT_SUCCESS;
}

void generate_movement_frame(IplImage *debug_frame, const IplImage *prev_frame, const IplImage *frame) {
  for (int y = 0; y < debug_frame->height; y++) {
    for (int x = 0; x < debug_frame->width * debug_frame->nChannels; x++) {
      if (prev_frame->imageData[y * frame->widthStep + x] - frame->imageData[y * frame->widthStep + x] != 0) {
        debug_frame->imageData[y * debug_frame->widthStep + x] = 255;
      } else {
        debug_frame->imageData[y * debug_frame->widthStep + x] = 0;
      }
    }
  }
}
