#include <stdio.h>
#include "matrix_2d.h"
#include "dtw.h"

int main(){
	// Matrix_2d m;
	// printf("float size: %ld, matrix size %ld\n", sizeof(Matrix_data_type), sizeof(Matrix_2d));
	// matrix_2d_init(&m, 5, 7, INIT_MODE_CONSTANT, 1.0, NULL, false);
	// printf("%d\n", matrix_2d_isInit(&m));
	// printf("%d\n", matrix_2d_isEmpty(&m));
	// matrix_2d_print(&m);

	// matrix_2d_resize(&m, 7, 10, INIT_MODE_CONSTANT, 5.0);
	// printf("%d\n", matrix_2d_isInit(&m));
	// printf("%d\n", matrix_2d_isEmpty(&m));
	// matrix_2d_print(&m);

	// matrix_2d_resize(&m, 10, 12, INIT_MODE_CONSTANT, 5.0);
	// printf("%d\n", matrix_2d_isInit(&m));
	// printf("%d\n", matrix_2d_isEmpty(&m));
	// matrix_2d_print(&m);

	// matrix_2d_resize(&m, 11, 15, INIT_MODE_CONSTANT, 7.0);
	// printf("%d\n", matrix_2d_isInit(&m));
	// printf("%d\n", matrix_2d_isEmpty(&m));
	// matrix_2d_print(&m);

	// matrix_2d_resize(&m, 20, 20, INIT_MODE_CONSTANT, 5.5);
	// printf("%d\n", matrix_2d_isInit(&m));
	// printf("%d\n", matrix_2d_isEmpty(&m));
	// matrix_2d_print(&m);

	// Matrix_2d nm;
	// matrix_2d_init(&nm, 30, 30, INIT_MODE_COPY_CONSTANT, 1.0, &m, true);
	// printf("%d\n", matrix_2d_isInit(&nm));
	// printf("%d\n", matrix_2d_isEmpty(&nm));
	// matrix_2d_print(&nm);
	//init s1
	Matrix_2d s1;
	matrix_2d_init(&s1, 10, 1, INIT_MODE_CONSTANT, 1.0, NULL, false);
	Matrix_2d s2;
	matrix_2d_init(&s2, 5, 1, INIT_MODE_CONSTANT, 1.0, NULL, false);

	float t1[10] = {1,1,2,2,2,7,2,3,4,5};
	float t2[5] = {1,2,3,4,5};
	for(int i = 0; i < 10; i++){
		s1.dptr[i][0] = t1[i];
	}
	for(int i = 0; i < 5; i++){
		s2.dptr[i][0] = t2[i];
	}
	// Matrix_data_type (*costFunction) (Matrix_data_type*, Matrix_data_type*, int);
	// costFunction = &euclidean_score;
	DTWManager dm;
	DTWManager_init(&dm, 1, &s1, &s2, &euclidean_score);
	DTWManager_print(&dm);
	float score;
	score = DTWManager_dtw(&dm);
	printf("score: %f \n", score);
	printf("\n");
	matrix_2d_resize(&s1, 12, 1, INIT_MODE_CONSTANT, 990);
	s1.dptr[10][0] = 5;
	s1.dptr[11][0] = 5;
	matrix_2d_resize(&s2, 7, 1, INIT_MODE_CONSTANT, 44.0);
	s2.dptr[5][0] = 5;
	s2.dptr[6][0] = 5;
	score = DTWManager_dtw(&dm);
	printf("score: %f \n", score);

	matrix_2d_resize(&s1, 14, 1, INIT_MODE_CONSTANT, 100.0);
	matrix_2d_print(&s1);
	s1.dptr[12][0] = 5;
	s1.dptr[13][0] = 5;
	matrix_2d_resize(&s2, 9, 1, INIT_MODE_CONSTANT, 200.0);
	s2.dptr[7][0] = 5;
	s2.dptr[8][0] = 6;
	score = DTWManager_dtw(&dm);
	printf("score: %f \n", score);
	// score = DTWManager_dtw(&dm);
	// DTWManager_print(&dm);
}