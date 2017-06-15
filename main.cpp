#include "cv.h"
#include "highgui.h"
#include "calibration.c"
#include "threshold.c"

void generate_movement_frame(IplImage *debug_frame, const IplImage *prev_frame, const IplImage *frame);

int main(int argc, char **argv) {
  CvCapture *capture = 0;
  IplImage *frame = 0;
  IplImage *prev_frame = 0;
  IplImage *result = 0;
  IplImage *arm = 0;

  capture = cvCaptureFromCAM(0);

  if(!capture) {
    perror("Error when reading steam");
    exit(EXIT_FAILURE);
  }

  // cvNamedWindow("Calibrate", 1);
  // calibration_t *cali = (calibration_t *) malloc(sizeof(calibration_t));
  // calibrate(capture, cali);

  calibration_t *c = (calibration_t *) malloc(sizeof(calibration_t));
  generic_calibration(c);

  cvNamedWindow("Dabby Bird", 1);
  cvNamedWindow("Arm Detection", 1);

  while (cvWaitKey(10) != 'q') {
    frame = cvQueryFrame(capture);
    cvShowImage("Dabby Bird", frame);

    if (frame) {
      if (prev_frame) {
        prev_frame = cvCloneImage(frame);

        // Arm detection
        arm = get_arm(frame, c);
        cvShowImage("Arm Detection", arm);
      } else {
        arm = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
      }
      prev_frame = cvCloneImage(frame);
    }
  }
  cvDestroyWindow("Dabby Bird");
  cvDestroyWindow("Arm Detection");
  cvReleaseCapture(&capture);

  // free(cali);
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
