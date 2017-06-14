#include "cv.h"
#include "highgui.h"

void generate_movement_frame(IplImage *debug_frame, const IplImage *prev_frame, const IplImage *frame);

int main(int argc, char **argv) {
  CvCapture *capture = 0;
  IplImage *frame = 0;
  IplImage *grey_frame = 0;
  IplImage *prev_frame = 0;
  IplImage *debug_frame = 0;
  IplImage *result = 0;
  IplImage *edge = 0;
  IplImage *prev_edge = 0;

  capture = cvCaptureFromCAM(0);

  if(!capture) {
    perror("Error when reading steam");
    exit(EXIT_FAILURE);
  }

  cvNamedWindow("Dabby Bird", 1);
  cvNamedWindow("Edge", 1);
  cvNamedWindow("Moving Edge", 1);

  while (cvWaitKey(10) != 'q') {
    frame = cvQueryFrame(capture);
    cvShowImage("Dabby Bird", frame);

    if (frame) {
      if (prev_frame) {
        prev_frame = cvCloneImage(edge);
        cvCvtColor(frame, grey_frame, CV_BGR2GRAY);
        cvCanny(grey_frame, edge, 200.0, 200.0, 3);
        if (prev_frame && edge) {
          generate_movement_frame(debug_frame, edge, prev_edge);
          cvShowImage("Moving Edge", debug_frame);
          cvShowImage("Edge", edge);
        }
      } else {
        debug_frame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
        grey_frame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
        edge = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
        prev_edge = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
      }
      prev_frame = cvCloneImage(frame);
    }
  }
  cvDestroyWindow("Dabby Bird");
  cvDestroyWindow("Edge");
  cvDestroyWindow("Moving Edge");
  cvReleaseCapture(&capture);

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
