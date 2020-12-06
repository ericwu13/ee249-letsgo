#include "matrix_2d.h"
#ifndef TEST_MATRIX_2D
#include "nrf_delay.h"
#endif 
/***************************************



****************************************/
static size_t get_array_memory_size(size_t k)
{
	return (size_t)(k*1.5);
}

static void init_row(Matrix_data_type* col_ptr, size_t scol, size_t ncol, Init_mode mode, Matrix_data_type con)
{
	if(mode == INIT_MODE_CONSTANT){
		for(int i = scol; i < (int)ncol; i++)
		{
			col_ptr[i] = con;
		}
	}
}

static void copy_row_from_matrix(Matrix_data_type* col_ptr, size_t ncol, size_t r, Matrix_2d* cm, Init_mode mode, Matrix_data_type con)
{
	//only check cm 
	//assume col_ptr is valid
	if(!matrix_2d_isInit(cm)) return;
	if(r >= cm->nrow) return;

	//determine r and c
	size_t col;
	if(ncol < cm->ncol){
		col = ncol;
	}
	else {
		col = cm->ncol;
	}

	memcpy((void*)col_ptr, (void*) cm->dptr[r], sizeof(Matrix_data_type) * col);

	if(mode == INIT_MODE_COPY_CONSTANT){
		init_row(col_ptr, col, ncol, INIT_MODE_CONSTANT, con);
	}
}


static void matrix_2d_ptr_copy(Matrix_2d* m, Matrix_2d* m_new){
	if(matrix_2d_isInit(m))
		matrix_2d_delete(m);

	m->dptr = m_new->dptr;
	m->row_size = m_new->row_size;
	m->col_size = m_new->col_size;
	m->nrow = m_new->nrow; 
	m->ncol = m_new->ncol;	
}

/***************************************

Public function

****************************************/

bool matrix_2d_isEmpty(Matrix_2d* m)
{
	if(!matrix_2d_isInit(m))
		return true;
	else
		return !(m->nrow | m->ncol);
}

bool matrix_2d_isInit(Matrix_2d* m)
{
	return m->dptr != NULL;
}

bool matrix_2d_getDim(Matrix_2d* m, size_t* nrow, size_t* ncol)
{
	if(!matrix_2d_isEmpty(m)){
		*nrow = m->nrow;
		*ncol = m->ncol;
	}
	else{
		printf("Error matrix not initialized!");
		return false;
	}
	return true;
}

void matrix_2d_delete(Matrix_2d* m){
	for(int r = 0; r < (int)(m->row_size); r++)
	{
		free(m->dptr[r]);
	}
	free(m->dptr);
	m->dptr = NULL;
	m->row_size = m->col_size = m->nrow = m->ncol = 0;	
}

/*
copy the row in corresponding matrix(cm)
col_ptr: row that wants the copied data in cm
cm:      matrix containing the wanted data
ncol:    col size of the row
r:       the rth number in cm to be copied
*/

uint8_t matrix_2d_init(Matrix_2d* m, size_t nrow, size_t ncol, Init_mode mode, Matrix_data_type con, Matrix_2d* cm, bool realsize)
{
	//init row
	if(nrow == 0 || ncol == 0){
		m->dptr = NULL;
		m->ncol = 0;
		m->nrow = 0;
		m->row_size = 0;
		m->col_size = 0;
		return 0;
	}
	size_t row_size = nrow;
	size_t col_size = ncol;
	if(!realsize){
	 	row_size = get_array_memory_size(nrow);
		col_size = get_array_memory_size(ncol);
	}

	Matrix_data_type** ptr = (Matrix_data_type**) calloc(row_size, sizeof(Matrix_data_type*));

	if(ptr == NULL){
		printf("matrix memory allocation error!\n");
		return -1;
	}

	//init col
	for(int r = 0; r < (int)nrow; r++){
		ptr[r] = (Matrix_data_type*) calloc(col_size, sizeof(Matrix_data_type));
		if(ptr[r] == NULL){
			printf("matrix memory allocation error!\n");
			// free previously allocated memory
			for(int j = r-1; j >= 0; j--)
			{
				free(ptr[j]);
			}
			free(ptr);
			return -1;
		}	
		// set initialized data
		if(mode == INIT_MODE_CONSTANT){
			init_row(ptr[r], 0, ncol, INIT_MODE_CONSTANT, con);
		}
		else if (mode != INIT_MODE_NOT_INITIALIZE){
			//todo copy matrix
			if(r < (int)(cm->nrow))
				copy_row_from_matrix(ptr[r], ncol, r, cm, mode, con);
			else if(mode == INIT_MODE_COPY_CONSTANT)
				init_row(ptr[r], 0, ncol, INIT_MODE_CONSTANT, con);
		}

	}
	m->ncol = ncol;
	m->nrow = nrow;
	m->row_size = row_size;
	m->col_size = col_size;
	m->dptr = ptr;
	return 0;
}

void matrix_2d_print(Matrix_2d* m){

	if(!matrix_2d_isInit(m)){
		printf("Uninitialized Matrix\n");
		return;
	}

	printf("Dim = (%d, %d), Max_Size = (%d, %d)\n", m->nrow, m->ncol, m->row_size, m->col_size);
	#ifndef TEST_MATRIX_2D
	nrf_delay_ms(200);
	#endif
	printf("[");
	for(int r = 0; r < (int)(m->nrow); r++){
		r == 0 ? printf("[") : printf(" [");
		for(int c = 0; c < (int)(m->ncol); c++){
			printf("%7.2f ", m->dptr[r][c]);
		}
		if(r + 1 == (int)(m->nrow)) printf("]");
		else printf("],\n");
		#ifndef TEST_MATRIX_2D
		nrf_delay_ms(200);
		#endif
	}
	printf("]\n");
}

uint8_t matrix_2d_resize(Matrix_2d* m, size_t nrow, size_t ncol, Init_mode mode, Matrix_data_type con)
{
	if(mode != INIT_MODE_CONSTANT){
		printf("resize does not support copy!");
		return -1;
	}

	if(!matrix_2d_isInit(m)){
		matrix_2d_init(m, nrow, ncol, INIT_MODE_CONSTANT, con, NULL, false);
		return 0;
	}


	//check memory size
	size_t row_size = get_array_memory_size(nrow);
	//size_t col_size = get_array_memory_size(ncol);

	if(/*nrow > m->row_size && */ncol > m->col_size){// both axis exceed original size, complete useless 
		Matrix_2d m_new;
		matrix_2d_init(&m_new, nrow, ncol, INIT_MODE_COPY_CONSTANT, con, m, false);
		matrix_2d_delete(m);
		matrix_2d_ptr_copy(m, &m_new);
		return 0;
	}
	else if(nrow > m->row_size){// column usable
		//col_size won't change
		//m->row_size increase to row_size 
		Matrix_data_type** ptr = (Matrix_data_type**) calloc(row_size, sizeof(Matrix_data_type*));
		if(ptr == NULL){
			printf("matrix memory allocation error!\n");
			return -1;
		}
		//copy old data
		memcpy((void*) ptr, (void*) m->dptr, sizeof(Matrix_data_type*) * m->nrow);
		//pad old dada
		for(int r = 0; r < (int)(m->nrow); r++){
			init_row(ptr[r], m->ncol, ncol, INIT_MODE_CONSTANT, con);
		}
		// create new row
		for(int r = m->nrow; r < (int)(nrow); r++)
		{
			ptr[r] = (Matrix_data_type*) calloc(m->col_size, sizeof(Matrix_data_type));
			if(ptr[r] == NULL){
				printf("matrix memory allocation error!\n");
				// free previously allocated memory
				for(int j = r-1; j >= 0; j--)
				{
					free(ptr[j]);
				}
				free(ptr);
				return -1;
			}	
			// set initialized data
			if(mode == INIT_MODE_CONSTANT){
				init_row(ptr[r], 0, ncol, INIT_MODE_CONSTANT, con);
			}			
		}
		free(m->dptr);
		m->ncol = ncol;
		m->nrow = nrow;
		m->row_size = row_size;
		m->dptr = ptr;
		return 0;
	}
	else if(ncol > m->col_size){//row usable //deprecated since i change the condition for both axis reset
		//row_size won't change
		//m->col_size increase to row_size 
		// Matrix_data_type** ptr = m->dptr;
		// //
		// for(r = 0; r < m->nrow)
		// {
		// 	Matrix_data_type* row_ptr = (Matrix_data_type*) calloc(col_size, sizeof(Matrix_data_type));
		// 	if(ptr[r] == NULL){
		// 		printf("matrix memory allocation error!\n");
		// 		// free previously allocated memory
		// 		matrix_2d_delete(m);
		// 		return -1;
		// 	}	

		// 	copy_row_from_matrix(row_ptr, ncol, r, m, mode, con);
		// 	free(ptr[r]);
		// 	ptr[r] = row_ptr;
		// }

		//lazy just copy
		Matrix_2d m_new;
		matrix_2d_init(&m_new, nrow, ncol, INIT_MODE_COPY_CONSTANT, con, m, false);
		matrix_2d_delete(m);
		matrix_2d_ptr_copy(m, &m_new);
		return 0;
	}
	else{// can inplace!

		Matrix_data_type** ptr = m->dptr;
		if(nrow <= (int)(m->nrow) && ncol <= (int)(m->ncol)){//just change nrow, ncol
			m->ncol = ncol;
			m->nrow = nrow;
			return 0;
		}

		for(int r = 0; r < (int)(m->nrow); r++){
			init_row(ptr[r], m->ncol, ncol, INIT_MODE_CONSTANT, con);
		}
		// create new row
		for(int r = m->nrow; r < (int)(nrow); r++)
		{
			ptr[r] = (Matrix_data_type*) calloc(m->col_size, sizeof(Matrix_data_type));
			if(ptr[r] == NULL){
				printf("matrix memory allocation error!\n");
				// free previously allocated memory
				for(int j = r-1; j >= 0; j--)
				{
					free(ptr[j]);
				}
				free(ptr);
				return -1;
			}	
			// set initialized data
			if(mode == INIT_MODE_CONSTANT){
				init_row(ptr[r], 0, ncol, INIT_MODE_CONSTANT, con);
			}			
		}
		m->ncol = ncol;
		m->nrow = nrow;
		return 0;
	}
	return 0;
}

void matrix_2d_reset_constant(Matrix_2d* m, Matrix_data_type con){
	for(int r = 0; r < (int)(m->nrow); r++){
		init_row(m->dptr[r], 0, m->ncol, INIT_MODE_CONSTANT, con);
	}
}