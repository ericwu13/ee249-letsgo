#ifndef DTW_H
#define DTW_H

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include "matrix_2d.h"



//comment out since we won't use this feature
//#define INCREMENTAL_SUPPORT

#define DTW_INF FLT_MAX 
#define score_t Matrix_data_type;

typedef Matrix_2d Signal;




typedef struct dtwmanager
{
	Matrix_2d d;
	Signal* signal1;
	Signal* signal2;
	int dim;
#ifdef INCREMENTAL_SUPPORT
	int n;
	int m;
#endif
	Matrix_data_type (*costFunction) (Matrix_data_type*, Matrix_data_type*, int);
} DTWManager ;

void DTWManager_print(DTWManager* dm);
void DTWManager_init(DTWManager* dm, int dim, Signal* signal1, Signal* signal2, Matrix_data_type (*costFunction) (Matrix_data_type*, Matrix_data_type*, int));
float DTWManager_dtw(DTWManager* dm);
void DTWManager_resetScore(DTWManager* dm);
#ifdef INCREMENTAL_SUPPORT
float DTWManager_incremental_dtw(DTWManager* dm, Signal* signal1, Signal* signal2);
float _incremental_dtw_oneside1(DTWManager* dm, Signal* signal1);
float _incremental_dtw_oneside2(DTWManager* dm, Signal* signal2);
#endif
Matrix_data_type euclidean_score(Matrix_data_type* dp1, Matrix_data_type* dp2, int dim);










#endif