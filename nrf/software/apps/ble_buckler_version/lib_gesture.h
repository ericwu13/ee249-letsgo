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

#define NUM_IMU_DATA 13
#define MAX_SIGNAL_LENGTH 20

#define label_t char
#define label_size sizeof(label_t)
#define length_t int
#define length_size sizeof(length_t)

#define DTW_INF FLT_MAX 
#define score_t Matrix_data_type;

extern const int LIBRARY_SIZE;
extern const char* filenames[];



typedef struct candidate {
	Matrix_2d data;
	label_t label;
	float threshold;
} Candidate;

typedef struct library {
	Candidate* c_array;
	Matrix_2d signal;
	size_t signal_length;
} Library;

Library lib_gesture;

void virtual_timer_init(void);
void virtual_timer_reset();
uint32_t read_timer(void);

void	 	candidate_init(Candidate* cand,int nrow, int ncol, label_t label, float threshold);
void 		candidate_debug(Candidate* cand);
uint8_t 	readLibFile(Candidate* cand, const char* filename, FIL* lib_file);

Library* 	preload_library();
Library* 	load_library(int idx);

Matrix_data_type euclidean_score(Matrix_data_type* dp1, Matrix_data_type* dp2, int dim);

uint8_t 	library_init(Library* lib, int size);
void 		library_delete(Library* lib);
void    	library_debug(Library* lib);
void 		library_push_signal(Library* lib, Matrix_data_type* datapoint);
label_t 	library_recognition(Library* lib);
void  		library_reset_signal(Library* lib);

// int         write_library_fds();
// int         write_candidate_fds(Candidate* cand, uint16_t record_key, uint16_t file_id);
// int         load_library_fds(int idx);
// int 			load_candidate_fds(Candidate* cand, uint16_t record_key, uint16_t file_id);
// int 			library_fds_init();
#endif