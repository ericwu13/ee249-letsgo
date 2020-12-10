#ifndef LIB_GESTURE_H
#define LIB_GESTURE_H

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "nrf_delay.h"
#include "display.h"
//#include "fds.h"

#define DTW_INF FLT_MAX 

#define NUM_IMU_DATA 13
#define MAX_SIGNAL_LENGTH 20
#define LIBRARY_SIZE 24

extern const float test_array[LIBRARY_SIZE][MAX_SIGNAL_LENGTH][NUM_IMU_DATA];
extern const float thresholds[LIBRARY_SIZE];
extern const char  gestures  [LIBRARY_SIZE];

float euclidean_score_dtw(const float* dp1, const float* dp2, int dim);
char dtw(float scoreMatrix[][MAX_SIGNAL_LENGTH], float signal[][NUM_IMU_DATA], int counter);


#endif