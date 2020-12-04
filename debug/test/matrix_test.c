#include <stdio.h>
#include "matrix_2d.h"

int main(){
	Matrix_2d m;
	printf("float size: %ld, matrix size %ld\n", sizeof(Matrix_data_type), sizeof(Matrix_2d));
	matrix_2d_init(&m, 5, 7, INIT_MODE_CONSTANT, 1.0, NULL, false);
	printf("%d\n", matrix_2d_isInit(&m));
	printf("%d\n", matrix_2d_isEmpty(&m));
	matrix_2d_print(&m);

	matrix_2d_resize(&m, 7, 10, INIT_MODE_CONSTANT, 5.0);
	printf("%d\n", matrix_2d_isInit(&m));
	printf("%d\n", matrix_2d_isEmpty(&m));
	matrix_2d_print(&m);

	matrix_2d_resize(&m, 10, 12, INIT_MODE_CONSTANT, 5.0);
	printf("%d\n", matrix_2d_isInit(&m));
	printf("%d\n", matrix_2d_isEmpty(&m));
	matrix_2d_print(&m);

	matrix_2d_resize(&m, 11, 15, INIT_MODE_CONSTANT, 7.0);
	printf("%d\n", matrix_2d_isInit(&m));
	printf("%d\n", matrix_2d_isEmpty(&m));
	matrix_2d_print(&m);

	matrix_2d_resize(&m, 20, 20, INIT_MODE_CONSTANT, 5.5);
	printf("%d\n", matrix_2d_isInit(&m));
	printf("%d\n", matrix_2d_isEmpty(&m));
	matrix_2d_print(&m);

	Matrix_2d nm;
	matrix_2d_init(&nm, 30, 30, INIT_MODE_COPY_CONSTANT, 1.0, &m, true);
	printf("%d\n", matrix_2d_isInit(&nm));
	printf("%d\n", matrix_2d_isEmpty(&nm));
	matrix_2d_print(&nm);
}