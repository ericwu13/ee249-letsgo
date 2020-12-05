#include "dtw.h"
#ifndef TEST_MATRIX_2D
#include "nrf_delay.h"
#endif


static inline float min_of_three(Matrix_data_type a, Matrix_data_type b, Matrix_data_type c){
	return fminf(a, fminf(b, c));
}

float euclidean_score(Matrix_data_type* dp1, Matrix_data_type* dp2, int dim){
	Matrix_data_type score = 0;
	for(int i = 0; i < dim; i++){
		Matrix_data_type diff = dp1[i] - dp2[i];
		score += diff*diff;
	}
	return sqrt(score);
}

void DTWManager_print(DTWManager* dm){
	printf("=========DTW MANAGER========\n");
	#ifndef TEST_MATRIX_2D
	nrf_delay_ms(200);
	#endif
	printf("Dim = %d\n", dm->dim);
	#ifndef TEST_MATRIX_2D
	nrf_delay_ms(200);
	#endif
	printf("Signal 1: \n");
	#ifndef TEST_MATRIX_2D
	nrf_delay_ms(200);
	#endif
	matrix_2d_print(dm->signal1);
	#ifndef TEST_MATRIX_2D
	nrf_delay_ms(200);
	#endif
	printf("Signal 2: \n");
	#ifndef TEST_MATRIX_2D
	nrf_delay_ms(200);
	#endif
	matrix_2d_print(dm->signal2);
	#ifndef TEST_MATRIX_2D
	nrf_delay_ms(200);
	#endif
	printf("Score Matrix: \n");
	#ifndef TEST_MATRIX_2D
	nrf_delay_ms(200);
	#endif
	matrix_2d_print(&(dm->d));
}

void DTWManager_init(DTWManager* dm, int dim, Signal* signal1, Signal* signal2, Matrix_data_type (*costFunction) (Matrix_data_type*, Matrix_data_type*, int)){
	dm->dim  = dim;
	dm->signal1 = signal1;
	dm->signal2 = signal2;
	dm->costFunction = costFunction;
	// ensure the pointer inside it is NULL
	matrix_2d_init(&(dm->d), 0, 0, INIT_MODE_CONSTANT, DTW_INF, NULL, false);
}

void DTWManager_reset_score(DTWManager* dm){
	if(matrix_2d_isInit(&(dm->d))){
		matrix_2d_delete(&(dm->d));
	}
}

float DTWManager_dtw(DTWManager* dm){
	Matrix_data_type score = DTW_INF;
	Matrix_2d* d = &(dm->d);
	int n_max = dm->signal1->nrow;
	int m_max = dm->signal2->nrow;

	if(matrix_2d_isInit(d)){
		#ifdef INCREMENTAL_SUPPORT
		score = DTWManager_incremental_dtw(dm);
		#else
		printf("Error! Score exists! Please reset score in advance!\n");
		#endif
		return score;
	}
	else{
		#ifdef INCREMENTAL_SUPPORT
		matrix_2d_init(d, n_max, m_max, INIT_MODE_CONSTANT, DTW_INF, NULL, false);
		#else
		matrix_2d_init(d, n_max, m_max, INIT_MODE_CONSTANT, DTW_INF, NULL, true);
		#endif		
	}


	for (int n = 0; n < n_max; n++){
		for(int m = 0; m < m_max; m++){
			Matrix_data_type match;// d->dptr[n-1][m-1]
			Matrix_data_type del;// d->dptr[n-1][m]
			Matrix_data_type insert;// d->dptr[n][m-1]

			//prevent out of array
			if(n != 0 && m != 0) {//safe
				match = d->dptr[n-1][m-1];
				del = d->dptr[n-1][m];
				insert = d->dptr[n][m-1];
			}
			else if(n != 0) {// m is 0
				match = DTW_INF;
				del = d->dptr[n-1][m];
				insert = DTW_INF;
			}
			else if (m != 0 ){// n is 0
				match = DTW_INF;
				del = DTW_INF;
				insert = d->dptr[n][m-1];
			}
			else{match = del = insert = 0;}// all zero, special case return 0

			d->dptr[n][m] = dm->costFunction(dm->signal1->dptr[n], dm->signal2->dptr[m], dm->dim) 
							+ min_of_three( match, del, insert);
		}
	}
	score = d->dptr[n_max-1][m_max-1];
	DTWManager_print(dm);
	#ifndef INCREMENTAL_SUPPORT
	DTWManager_reset_score(dm);
	#endif
	//DTWManager_print(dm);
	return score;
}