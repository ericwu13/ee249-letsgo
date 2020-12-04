#ifndef LIB_GESTURE_H
#define LIB_GESTURE_H

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "simple_logger.h"
#include "matrix_2d.h"
#include "nrf_delay.h"

#define label_t char
#define label_size sizeof(label_t)
#define length_t int
#define length_size sizeof(length_t)

extern const int LIBRARY_SIZE;
extern const char* filenames[];

typedef struct candidate {
	Matrix_2d data;
	label_t label;
	float threshold;
} Candidate;

typedef struct library {
	Candidate* c_array;
} Library;

Library lib_gesture;

uint8_t 	preload_library();
uint8_t 	library_init(Library* lib, int size);
uint8_t 	readLibFile(Candidate* cand, const char* filename);

void	 	candidate_init(Candidate* cand,int nrow, int ncol, label_t label, float threshold);

void 		library_delete(Library* lib);

void    	library_debug(Library* lib);
void 		candidate_debug(Candidate* cand);



#endif