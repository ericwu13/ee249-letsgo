#ifndef MATRIX_2D_H
#define MATRIX_2D_H


#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define Matrix_data_type float

#ifdef TEST_MATRIX_2D
#define uint8_t char
#endif 

typedef enum init_mode{
	INIT_MODE_NOT_INITIALIZE = 0,
	INIT_MODE_COPY,
	INIT_MODE_COPY_CONSTANT,
	INIT_MODE_CONSTANT
} Init_mode;

typedef struct {
	Matrix_data_type** dptr;
	size_t row_size;
	size_t col_size;
	size_t nrow;
	size_t ncol;
} Matrix_2d;

bool matrix_2d_isEmpty(Matrix_2d* m);

bool matrix_2d_isInit(Matrix_2d* m);

bool matrix_2d_getDim(Matrix_2d* m, size_t* nrow, size_t* ncol);

void matrix_2d_delete(Matrix_2d* m);

void matrix_2d_print(Matrix_2d* m);

uint8_t matrix_2d_init(Matrix_2d* m, size_t nrow, size_t ncol, Init_mode mode, 
						Matrix_data_type con, Matrix_2d* cm, bool realsize);

uint8_t matrix_2d_resize(Matrix_2d* m, size_t nrow, size_t ncol, Init_mode mode, Matrix_data_type con);

#endif