#include "lib_gesture.h"

// const int LIBRARY_SIZE = 24;
// const char* filenames[]  = {"F_1.bin", "F_2.bin", "F_3.bin", "F_4.bin",
// 							"L_1.bin", "L_2.bin", "L_3.bin", "L_4.bin",
// 							"R_1.bin", "R_2.bin", "R_3.bin", "R_4.bin",
// 							"B_1.bin", "B_2.bin", "B_3.bin", "B_4.bin",
// 							"S_1.bin", "S_2.bin", "S_3.bin", "S_4.bin",
// 							"G_1.bin", "G_2.bin", "G_3.bin", "G_4.bin"
// 						 };
const int LIBRARY_SIZE = 2;
const char* filenames[]  = {"F_1.bin", "S_1.bin"
						 };
uint8_t preload_library()
{
	uint8_t res;
	res = library_init(&lib_gesture, LIBRARY_SIZE);
	if(res){
		printf("Library init fail... aborted..."); return res;
	}
	for(int i = 0; i< LIBRARY_SIZE; i++){
		res = readLibFile(&(lib_gesture.c_array[i]), filenames[i]);
		if(res){
			printf("read file failed... aborted..."); return res;
		}
	}
	printf("Library preload successfully!\n");
	library_debug(&lib_gesture);
	return res;
}

uint8_t library_init(Library* lib, int size){
	lib->c_array = (Candidate*) calloc(size, sizeof(Candidate));
	matrix_2d_init(&(lib->signal), 0, 0, INIT_MODE_NOT_INITIALIZE, NULL, false);
	if(lib->c_array == NULL){
		printf("Library initialize error!\n");
		return -1;
	}
	return 0;
}

void library_delete(Library* lib){
	for(int i = 0; i < LIBRARY_SIZE; i++){
		Matrix_2d* libmat = &(lib->c_array[i].data);
		if(matrix_2d_isInit(libmat))
			matrix_2d_delete(libmat);
	}
	free(lib->c_array);
	if(matrix_2d_isInit(&(lib->signal)))
		matrix_2d_delete(&(lib->signal));
}

uint8_t readLibFile(Candidate* cand, const char* filename){
	printf("Loading data in File: %s.....\n", filename);
	uint8_t res;

	res = openfile(filename, "a,r");

	if(res){
		printf("File: %s cannot open!\n", filename); return -1;
	}

	res = moveptr_head();
	if(res){
		printf("File: %s error \n", filename); return -1;
	}

	label_t label;
	float threshold;
	length_t length_c, length_r;

	//read label
	res = readfile((uint8_t*) &(label)  , sizeof(label_t)); 

	if(res){
		printf("File: %s label error \n", filename); return -1;
	}	
	//read threshold
	res = readfile((uint8_t*) &(threshold), sizeof(float));

	if(res){
		printf("File: %s threshold error \n", filename); return -1;
	}	
	//read length?
	
	res |= readfile((uint8_t*) &(length_r), sizeof(length_t));
	res |= readfile((uint8_t*) &(length_c), sizeof(length_t));
	if(res){
		printf("File: %s length error \n", filename); return -1;
	}		

	candidate_init(cand, length_r, length_c, label, threshold);
	printf("Data info: %d data with dim %d\n", length_r, length_c);
	//read data
	for(int i = 0; i < length_r; i++){
		res = readfile((uint8_t*) (cand->data.dptr[i]), sizeof(float)*length_c);
		if(res){
			printf("Load data error! row: %d\n", i); return -1;
		}
	}

	res = closefile();
	if(res){
		printf("Close file error\n");  return -1;
	}	
	printf("Load successfully!\n");
	return 0;
}

void candidate_init(Candidate* cand,int nrow, int ncol, label_t label, float threshold){
	matrix_2d_init(&(cand->data), nrow, ncol, INIT_MODE_NOT_INITIALIZE, 0, 0, true);
	cand->label = label;
	cand->threshold = threshold;
}

void library_debug(Library* lib){
	printf("\n=======LIBRARY_INFO======\n");
	nrf_delay_ms(100);
	printf("Number of Candidate: %d\n", LIBRARY_SIZE);
	nrf_delay_ms(100);
	for(int i = 0; i < LIBRARY_SIZE; i++){
		nrf_delay_ms(100);
		printf("Candidate %d: ", i);
		candidate_debug(&(lib->c_array[i]));
		printf("\n");
		nrf_delay_ms(100);
	}
	printf("Singal info:\n");
	nrf_delay_ms(100);
	matrix_2d_print(&(lib->signal));
}

void candidate_debug(Candidate* cand){
	nrf_delay_ms(100);
	printf("label: %c ", cand->label);
	nrf_delay_ms(100);
	printf("threshold: %f ", cand->threshold);
	matrix_2d_print(&(cand->data));
}

void 	library_push_signal(Library* lib, Matrix_data_type* datapoint){

	matrix_2d* signal = &(lib->signal);
	if(!matrix_2d_isInit(signal)){
      matrix_2d_init(signal, 1, NUM_IMU_DATA, INIT_MODE_CONTANT, 0, NULL, false);
    }
    else{
      matrix_2d_resize(signal, signal->nrow + 1, NUM_IMU_DATA, INIT_MODE_CONTANT, 0);
    }
    memcpy(signal.dtpr[signal.nrow - 1], datapoint, NUM_IMU_DATA*sizeof(Matrix_data_type))
}

label_t library_recognition(Library* lib){
	float score;
	for(int i = 0; i < LIBRARY_SIZE; i++){
		//
		Candidate* cand = &(lib->c_array[i]);
		DTWManager_init(&(lib->dm), NUM_IMU_DATA,  &signal, &(cand->data), &euclidean_score);
		score = DTWManager_dtw(&(lib->dm));
		if(score > cand->threshold){
			return cand->label;
		}
	}
	return 0;//no result;
}
void  	library_reset_signal(Library* lib){
	matrix_2d_resize(&(lib->signal), 0, NUM_IMU_DATA, INIT_MODE_CONTANT, 0);
}