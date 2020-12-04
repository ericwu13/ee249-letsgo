#ifndef DTW_H
#define DTW_H

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix_2d.h"

#define DTW_INFINITY FLT_MAX 
#define Matrix_data_type float


enum {
	INIT_MODE_NOT_INITIALIZE = 0,
	INIT_MODE_COPY,
	INIT_MODE_COPY_CONTANT,
	INIT_MODE_CONSTANT
} Init_mode;

typedef struct
{
	Matrix_2d* d;
	Matrix_2d* signal1;
	Matrix_2d* signal2;
	size_t dim;
	size_t n;
	size_t m;
	void* costFunction;
} DTWManager ;

void DTWManager_init(DTWManager* dm, Matrix_2d* d, Matrix_2d* signal1, Matrix_2d* signal2, void* costFunction);
float perform_dtw(DTWManager* dm);
float incremental_dtw(DTWManager* dm, Matrix_2d* signal1, Matrix_2d* signal2);
float _incremental_dtw_oneside1(DTWManager* dm, Matrix_2d* signal1);
float _incremental_dtw_oneside2(DTWManager* dm, Matrix_2d* signal2);




#endif