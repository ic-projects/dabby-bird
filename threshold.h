#ifndef THRESHOLD_H
#define THRESHOLD_H
#include "calibration.h"

IplImage *get_arm(IplImage *frame, calibration_t *c);
IplImage *get_arm_variable(IplImage *frame, calibration_t *c);

#endif
