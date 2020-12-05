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
	matrix_2d_init(&s1, 20, 13, INIT_MODE_CONSTANT, 1.0, NULL, false);
	Matrix_2d s2;
	matrix_2d_init(&s2, 20, 13, INIT_MODE_CONSTANT, 1.0, NULL, false);

	// Matrix_data_type (*costFunction) (Matrix_data_type*, Matrix_data_type*, int);
	// costFunction = &euclidean_score;
	DTWManager dm;
	DTWManager_init(&dm, 13, &s1, &s2, &euclidean_score);
	DTWManager_print(&dm);
	float score;
	score = DTWManager_dtw(&dm);
	matrix_2d_resize(&s1, 40, 13, INIT_MODE_CONSTANT, 5.0);
	matrix_2d_resize(&s2, 21, 13, INIT_MODE_CONSTANT, 5.0);
	score = DTWManager_dtw(&dm);
	DTWManager_print(&dm);
}