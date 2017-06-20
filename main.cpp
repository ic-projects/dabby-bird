/**
 * @file main.cpp
 * @brief Main file for motion-controlled games.
 */

#include <math.h>
#include <unistd.h>
#include "cv.h"
#include "highgui.h"
#include "uchar_array.c"
#include "calibration.c"
#include "threshold.c"
#include "detection.c"
#include "flappy_bird.c"

void generate_movement_frame(IplImage *debug_frame, const IplImage *prev_frame, const IplImage *frame);

int main(int argc, char **argv) {
  CvCapture *capture = 0;
  IplImage *frame = 0;
  IplImage *prev_frame = 0;
  IplImage *result = 0;
  IplImage *arm = 0;
  CvScalar red = cvScalar(255, 0, 0);
  capture = cvCaptureFromCAM(0);
  hands_t *hands = init_hands();
  bool is_down = false;

  if (!capture) {
    perror("Error when reading stream");
    exit(EXIT_FAILURE);
  }

  calibration_t *c = (calibration_t *) malloc(sizeof(calibration_t));
  calibrate(capture, c);

  cvNamedWindow("Arm Detection", 1);
  cvNamedWindow("BW Matte", 1);

  object_list_t *objects = init_game();
  int is_alive = 1;
  clock_t last_frame = clock();

  while (cvWaitKey(10) != 'q') {
    frame = cvQueryFrame(capture);

    if (frame) {
      if (prev_frame) {
        cvReleaseImage(&prev_frame);
        prev_frame = cvCloneImage(frame);

        cvCvtColor(frame, frame, CV_BGR2HSV);
        cvReleaseImage(&arm);
        arm = get_arm(frame, c);
        cv::Mat image1 = cv::cvarrToMat(arm, false);
        cv::medianBlur(image1, image1, 11);
        arm = cvCreateImage(cvSize(image1.cols,image1.rows),8,arm->nChannels);
        IplImage ipltemp=image1;
        cvCopy(&ipltemp,arm);
        cvCvtColor(frame, frame, CV_HSV2BGR);
        detect_hands(arm, hands);

        int half = frame->height / 2;

        if (is_down && hands->left_y < half && hands->right_y < half) {
          is_down = false;
        } else if (!is_down && hands->left_y > half && hands->right_y > half) {
          is_down = true;
          for_all(objects, flap);
        }

        if (is_alive && clock() - last_frame >= 50 * 1000) {
          last_frame = clock();
          render_game(objects);
        }
        char c = 0;

        c = getch();
        if (c == ' ') {
          for_all(objects, flap);
        }

        if (c == 'r' || c == 'R') {
          is_alive = 1;
          free_object_list(objects);
          objects = init_game();
        }

        if (bird_coll(objects)) {
          is_alive = 0;
        }

        if (get_elem(objects, bird)->point.y > HEIGHT - 10) {
          get_elem(objects, bird)->velocity.y = 0;
          get_elem(objects, bird)->point.y = HEIGHT-10;
        }

        cvCircle(frame, cvPoint(hands->left_x, hands->left_y), 10, red, 15);
        cvCircle(frame, cvPoint(hands->right_x, hands->right_y), 10, red, 15);
        cvShowImage("BW Matte", arm);

      } else {
        arm = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
      }

      cvReleaseImage(&prev_frame);
      prev_frame = cvCloneImage(frame);
      cvFlip(frame, frame, 1);
      cvShowImage("Arm Detection", frame);
    }
  }

  sleep(5);
  endwin();
  printf("\nYou died!\n");
  for_all(objects, print_object);

  cvDestroyWindow("Arm Detection");
  cvDestroyWindow("BW Matte");
  cvReleaseCapture(&capture);
  cvReleaseImage(&frame);
  cvReleaseImage(&prev_frame);
  cvReleaseImage(&result);
  cvReleaseImage(&arm);

  free_object_list(objects);
  free(hands);
  free(c);

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
