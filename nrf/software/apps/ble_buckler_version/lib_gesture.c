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
const char* filenames[]  = { "S_1.bin", "F_1.bin"};
FIL lib_files[2];

float euclidean_score(Matrix_data_type* dp1, Matrix_data_type* dp2, int dim){
	Matrix_data_type score = 0;
	for(int i = 0; i < dim; i++){
		Matrix_data_type diff = dp1[i] - dp2[i];
		score += diff*diff;
	}
	return sqrt(score);
}

void virtual_timer_init(void) {
  // Place your timer initialization code here
    NRF_TIMER4->BITMODE = 0x3;
    NRF_TIMER4->PRESCALER = 0x4;
    NRF_TIMER4->TASKS_CLEAR = 0x1; 
    NRF_TIMER4->TASKS_START = 0x1;
}

void virtual_timer_reset(){
    NRF_TIMER4->TASKS_CLEAR = 0x1; 
    NRF_TIMER4->TASKS_START = 0x1; 
}

uint32_t read_timer(void) {
  NRF_TIMER4->TASKS_CAPTURE[1] = 0x1;
  uint32_t timer_value = NRF_TIMER4 -> CC[1];
  // Should return the value of the internal counter for TIMER4
  return timer_value;
}


Library* preload_library()
{
	uint8_t res;
	res = library_init(&lib_gesture, LIBRARY_SIZE);
	if(res){
		printf("Library init fail... aborted..."); return NULL;
	}
	for(int i = 0; i< LIBRARY_SIZE; i++){
		res = readLibFile(&(lib_gesture.c_array[i]), filenames[i], &(lib_files[i]));
		if(res){
			printf("read file failed... aborted..."); return NULL;
		}
	}
	printf("Library preload successfully!\n");
	//library_debug(&lib_gesture);
	return &lib_gesture;
}

Library* load_library(int idx){
	
	if(idx >= LIBRARY_SIZE){
		printf("Index out of range!\n");
	}
	int res = readLibFile(&(lib_gesture.c_array[0]), filenames[idx], &(lib_files[idx]));
	if(res){
			printf("read file failed... aborted..."); return NULL;
	}	
	//printf("Library %d load successfully!\n", idx);

	//library_debug(&lib_gesture);
	return &lib_gesture;
}

uint8_t library_init(Library* lib, int size){
	int res = mount_sd();
	if(res){
		printf("SD card Mounting error\n");
		return -1;
	}

	
	for(int i = 0; i < LIBRARY_SIZE; i++){	
		res = openfile_complete(&(lib_files[i]), filenames[i], "a,r");
		if(res){
			printf("File: %s cannot open!\n", filenames[i]); return -1;
		}
	}

	lib->c_array = (Candidate*) calloc(size, sizeof(Candidate));
	for(int i = 0; i < size; i++){
		matrix_2d_init(&(lib->c_array[i].data), 0, 0, INIT_MODE_NOT_INITIALIZE, 0, 0, true);
	}
	//matrix_2d_init(&(lib->signal), 40, NUM_IMU_DATA, INIT_MODE_NOT_INITIALIZE, 0, NULL, false);
	lib->signal_length = 0;
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

uint8_t readLibFile(Candidate* cand, const char* filename, FIL* lib_file){
	//printf("Loading data in File: %s.....\n", filename);
	uint8_t res;
	//virtual_timer_reset();
	res = moveptr_head_complete(lib_file);
	if(res){
		printf("File: %s error \n", filename); return -1;
	}

	label_t label;
	float threshold;
	length_t length_c, length_r;

	//read label
	res = readfile_complete(lib_file, (uint8_t*) &(label)  , sizeof(label_t)); 

	if(res){
		printf("File: %s label error \n", filename); return -1;
	}	
	//read threshold
	res = readfile_complete(lib_file, (uint8_t*) &(threshold), sizeof(float));

	if(res){
		printf("File: %s threshold error \n", filename); return -1;
	}	
	//read length?
	
	res |= readfile_complete(lib_file, (uint8_t*) &(length_r), sizeof(length_t));
	res |= readfile_complete(lib_file, (uint8_t*) &(length_c), sizeof(length_t));
	if(res){
		printf("File: %s length error \n", filename); return -1;
	}		

	candidate_init(cand, length_r, length_c, label, threshold);
	//printf("Data info: %d data with dim %d\n", length_r, length_c);
	//read data

	for(int i = 0; i < MAX_SIGNAL_LENGTH; i++){
		res = readfile_complete(lib_file, (uint8_t*) (cand->data.dptr[i]), sizeof(float)*length_c);
		if(res){
			printf("Load data error! row: %d\n", i); return -1;
		}
	}
	//uint32_t time = read_timer();
    //printf("Read file Time elapsed: %ld\n", time);
	// res = closefile(lib_files);
	// if(res){
	// 	printf("Close file error\n");  return -1;
	// }	
	//printf("Load successfully!\n");
	return 0;
}

void candidate_init(Candidate* cand,int nrow, int ncol, label_t label, float threshold){
	if(!matrix_2d_isInit(&(cand->data)))
		matrix_2d_init(&(cand->data), MAX_SIGNAL_LENGTH, NUM_IMU_DATA, INIT_MODE_NOT_INITIALIZE, 0, 0, true);
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
	//matrix_2d_print(&(lib->signal));
}

void candidate_debug(Candidate* cand){
	nrf_delay_ms(100);
	printf("label: %c ", cand->label);
	nrf_delay_ms(100);
	printf("threshold: %f ", cand->threshold);
	matrix_2d_print(&(cand->data));
}

// void 	library_push_signal(Library* lib, Matrix_data_type* datapoint){
// 	printf("Pushing signal....\n");
// 	nrf_delay_ms(500);
// 	Matrix_2d* signal = &(lib->signal);
// 	// if(!matrix_2d_isInit(signal)){
// 	// 	printf("Init\n");
//  //      matrix_2d_init(signal, 1, NUM_IMU_DATA, INIT_MODE_NOT_INITIALIZE, 0, NULL, false);
//  //    }
//  //    else{
//  //    	printf("Resize to %d\n", signal->nrow + 1);
//  //    	nrf_delay_ms(200);
//  //      matrix_2d_resize(signal, signal->nrow + 1, NUM_IMU_DATA, INIT_MODE_CONSTANT, 0);
//  //      printf("Maxsize: %d, %d\n", signal->row_size, signal->col_size);
//  //    }
//  	memcpy(signal->dptr[lib->signal_length - 1], datapoint, NUM_IMU_DATA*sizeof(Matrix_data_type));
//  	lib->signal_length++;
//     //matrix_2d_print(signal);
// }

// label_t library_recognition(Library* lib){
// 	float score;
// 	for(int i = 0; i < LIBRARY_SIZE; i++){
// 		printf("Recognition running....%d\n", i);
// 		nrf_delay_ms(500);
// 		Candidate* cand = &(lib->c_array[i]);
// 		DTWManager_init(&(lib->dm), NUM_IMU_DATA,  &(lib->signal), &(cand->data), &euclidean_score);
// 		printf("DTWManager init complete %d\n", i);
// 		nrf_delay_ms(500);
// 		score = DTWManager_dtw(&(lib->dm), lib->signal_length, cand->data.nrow);
// 		//DTWManager_print(&(lib->dm));
// 		printf("Gesture Label: %c Score: %f\n", cand->label, score);
// 		nrf_delay_ms(500);
// 		if(score < cand->threshold){
// 			return cand->label;
// 		}
// 	}
// 	return 0;//no result;
// }
void  	library_reset_signal(Library* lib){
	//matrix_2d_resize(&(lib->signal), 0, NUM_IMU_DATA, INIT_MODE_NOT_INITIALIZE, 0);
	lib->signal_length = 0;
	//matrix_2d_print(&(lib->signal));
}

// static void fds_evt_handler(fds_evt_t const * p_fds_evt)
// {
//     switch (p_fds_evt->id)
//     {
//         case FDS_EVT_INIT:
//             if (p_fds_evt->result != FDS_SUCCESS)
//             {
//                 printf("FDS Failed!\n");
//             }
//             break;
//         default:
//             break;
//     }
//     fds_lock = false;
// }
// int library_fds_init(){
// 	ret_code_t ret;

// 	ret = fds_register(fds_evt_handler);
// 	if (ret != FDS_SUCCESS)
// 	{
// 	  printf("Register FDS event handler error!\n");
// 	}
// 	fds_lock = true;
// 	ret = fds_init();
// 	if (ret != FDS_SUCCESS)
// 	{
// 	  printf("FDS initialization error!\n");
// 	}
// 	while(fds_lock){};	
// 	printf("Removing old files...........\n");
// 	for(int i = 0; i < LIBRARY_SIZE; i++){
// 		uint16_t file_id = (uint16_t)i;//
// 		fds_lock = true;
// 		ret = fds_file_delete(file_id);
// 		if (ret != FDS_SUCCESS)
// 		{
// 		    printf("Remove old files fail!\n");
// 		    return ret;
// 		}
// 		while(fds_lock){};	
// 	}
// 	fds_lock = true;
// 	ret = fds_gc();
// 	if (ret != FDS_SUCCESS)
// 	{
// 	    printf("GC fail!\n");
// 	    return ret;
// 	}
// 	while(fds_lock){};	
// 	printf("FDS initialization successfully!\n");
// 	return ret;
// }


// int  write_library_fds(){
// 	int res;
// 	for(int i = 0; i < LIBRARY_SIZE; i++){
// 		load_library(i);
// 		Candidate* cand = &(lib_gesture.c_array[0]);
// 		///
// 		uint16_t file_id = (uint16_t)i;//

// 		res = write_candidate_fds(cand, RECORD_START_KEY, file_id);
// 		if(res){
// 			printf("Write Libary %d to FDS failed!\n", i);
// 			return -1;
// 		}
// 	}
// 	return 0;
// }
// int        load_library_fds(int idx){
// 	Candidate* cand = &(lib_gesture.c_array[0]);
// 	uint16_t file_id = (uint16_t)idx;
// 	int res = load_candidate_fds(cand, RECORD_START_KEY, file_id);
// 	if(res){
// 		printf("Load Library from FDS failed!\n"); return -1;
// 	}
// 	return 0;
// }

// int  	load_candidate_fds(Candidate* cand, uint16_t record_key, uint16_t file_id){
// 	fds_flash_record_t  flash_record;
// 	fds_record_desc_t   record_desc;
// 	fds_find_token_t    ftok;
// 	/* It is required to zero the token before first use. */
// 	memset(&ftok, 0x00, sizeof(fds_find_token_t));
// 	memset(&record_desc, 0x00, sizeof(fds_record_desc_t));
// 	/* Loop until all records with the given key and file ID have been found. */
// 	fds_record_find(file_id, record_key++, &record_desc, &ftok);
//     if (fds_record_open(&record_desc, &flash_record) != FDS_SUCCESS) {printf("Open FDS File error!\n"); return -1;}
//     /* Access the record through the flash_record structure. */
//     uint32_t tmplabel = *(uint32_t*)(flash_record.p_data);
//     cand->label = (char)tmplabel;
//     /* Close the record when done. */
//     if (fds_record_close(&record_desc) != FDS_SUCCESS)   {printf("Close FDS File error!\n");return -1;}

//  	memset(&ftok, 0x00, sizeof(fds_find_token_t));
// 	memset(&record_desc, 0x00, sizeof(fds_record_desc_t));   
// 	fds_record_find(file_id, record_key++, &record_desc, &ftok);
//     if (fds_record_open(&record_desc, &flash_record) != FDS_SUCCESS) {printf("Open FDS File error!\n");return -1;}
//     cand->threshold = *(score_t*)(flash_record.p_data);
//     if (fds_record_close(&record_desc) != FDS_SUCCESS)  {printf("Close FDS File error!\n");return -1;}

//  	memset(&ftok, 0x00, sizeof(fds_find_token_t));
// 	memset(&record_desc, 0x00, sizeof(fds_record_desc_t));   
// 	fds_record_find(file_id, record_key++, &record_desc, &ftok);
//     if (fds_record_open(&record_desc, &flash_record) != FDS_SUCCESS) {printf("Open FDS File error!\n");return -1;}
//     uint32_t nrow = *(uint32_t*)(flash_record.p_data);
//     if (fds_record_close(&record_desc) != FDS_SUCCESS)  {printf("Close FDS File error!\n");return -1;}

//  	memset(&ftok, 0x00, sizeof(fds_find_token_t));
// 	memset(&record_desc, 0x00, sizeof(fds_record_desc_t));   
// 	fds_record_find(file_id, record_key++, &record_desc, &ftok);
//     if (fds_record_open(&record_desc, &flash_record) != FDS_SUCCESS) {printf("Open FDS File error!\n");return -1;}
//     uint32_t ncol = *(uint32_t*)(flash_record.p_data);
//     if (fds_record_close(&record_desc) != FDS_SUCCESS)  {printf("Close FDS File error!\n");return -1;}

//     for(int i = 0; i < MAX_SIGNAL_LENGTH; i++){
// 	 	memset(&ftok, 0x00, sizeof(fds_find_token_t));
// 		memset(&record_desc, 0x00, sizeof(fds_record_desc_t));   
// 		fds_record_find(file_id, record_key++, &record_desc, &ftok);
// 	    if (fds_record_open(&record_desc, &flash_record) != FDS_SUCCESS) {printf("Open FDS File error!\n");return -1;}
// 	    memcpy(&(cand->data.dptr[i]), flash_record.p_data, sizeof(Matrix_data_type) * ncol);
// 	    if (fds_record_close(&record_desc) != FDS_SUCCESS)  {printf("Close FDS File error!\n");return -1;}    	
//     }
//     return 0;
// }

// int        write_candidate_fds(Candidate* cand, uint16_t record_key, uint16_t file_id){

// 	fds_record_t        record;
// 	fds_record_desc_t   record_desc;
// 	uint32_t tmplabel = (cand->label);

// 	record.file_id           = file_id;
// 	record.key               = record_key++;
// 	record.data.p_data       = &tmplabel;
// 	record.data.length_words = 1;   /* one word is four bytes. */

// 	ret_code_t rc;
// 	fds_lock = true;
// 	rc = fds_record_write(&record_desc, &record);
// 	if (rc != FDS_SUCCESS)
// 	{
// 	    printf("write error!\n"); return -1;
// 	}
// 	while(fds_lock){};

// 	record.file_id           = file_id;
// 	record.key               = (record_key++);
// 	record.data.p_data       = &(cand->threshold);
// 	record.data.length_words = sizeof(score_t);   

// 	fds_lock = true;
// 	rc = fds_record_write(&record_desc, &record);
// 	if (rc != FDS_SUCCESS)
// 	{
// 	    printf("write error!\n"); return -1;
// 	}
// 	while(fds_lock){};	

// 	record.file_id           = file_id;
// 	record.key               = (record_key++);
// 	record.data.p_data       = &(cand->data.nrow);
// 	record.data.length_words = 1;   

// 	fds_lock = true;
// 	rc = fds_record_write(&record_desc, &record);
// 	if (rc != FDS_SUCCESS)
// 	{
// 	    printf("write error!\n"); return -1;
// 	}
// 	while(fds_lock){};	

// 	record.file_id           = file_id;
// 	record.key               = (record_key++);
// 	record.data.p_data       = &(cand->data.ncol);
// 	record.data.length_words = 1;   

// 	fds_lock = true;
// 	rc = fds_record_write(&record_desc, &record);
// 	if (rc != FDS_SUCCESS)
// 	{
// 	    printf("write error!\n"); return -1;
// 	}
// 	while(fds_lock){};		

// 	for(int i = 0; i < MAX_SIGNAL_LENGTH; i++){
// 		record.file_id           = file_id;
// 		record.key               = (record_key++);
// 		record.data.p_data       = &(cand->data.dptr[i]);
// 		record.data.length_words = cand->data.ncol;   

// 		fds_lock = true;
// 		rc = fds_record_write(&record_desc, &record);
// 		if (rc != FDS_SUCCESS)
// 		{
// 		    printf("write error!\n"); return -1;
// 		}
// 		while(fds_lock){};
// 	}
// 	printf("Candidate written!\n");
// 	return 0;
// }