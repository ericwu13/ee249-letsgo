#include "lib_gesture_flash.h"

const float thresholds[LIBRARY_SIZE] = {25,25,25};

const char  gestures  [LIBRARY_SIZE] = "LLL";


const float training_data[LIBRARY_SIZE][MAX_SIGNAL_LENGTH][NUM_IMU_DATA] = {
    {
        {-0.57, -0.39, 1.31, -6.78, -72.90, -4.87, -1.18, -0.88, 2.93, 1.89, 1.67, 1.55, 1.60},
{0.21, -0.96, 0.35, 0.39, 10.81, -0.90, 0.49, -2.18, 0.81, 1.91, 1.66, 1.50, 1.56},
{0.18, -0.93, 0.43, 5.60, 2.74, 2.51, 0.41, -2.15, 0.99, 1.89, 1.66, 1.57, 1.61}
    },
    {
        {-0.59, -0.91, 1.38, -1.36, -191.99, 49.24, -1.33, -2.08, 3.03, 1.92, 1.64, 1.51, 1.64},
{0.26, -0.93, 0.33, 0.68, 19.64, -0.58, 0.59, -2.17, 0.78, 1.94, 1.73, 1.54, 1.55},
{0.27, -0.95, 0.33, -13.49, -0.25, -11.52, 0.62, -2.18, 0.76, 1.94, 1.68, 1.59, 1.56}
    },
    {
        {-0.50, -1.02, 0.58, 173.90, -286.72, 153.82, -1.16, -2.55, 1.51, 1.93, 1.72, 1.51, 1.56},
{0.24, -0.83, 0.35, -24.45, -0.98, -8.10, 0.55, -1.88, 0.81, 1.95, 1.66, 1.58, 1.61},
{0.23, -0.93, 0.35, -12.45, 5.97, -3.02, 0.53, -2.11, 0.78, 1.90, 1.69, 1.60, 1.55}
    },
};

static inline float min_of_three(float a, float b, float c){
  return fminf(a, fminf(b, c));
}

float euclidean_score_dtw(const float* dp1, const float* dp2, int dim){
	float score = 0;
	for(int i = 0; i < dim; i++){
        if(i <=5 && i >= 3)
            continue;
		float diff = dp1[i] - dp2[i];
		score += diff*diff;
	}
	return sqrt(score);
}

char dtw(float scoreMatrix[][MAX_SIGNAL_LENGTH], float signal[][NUM_IMU_DATA], int counter){
    // printf("DTW start! counter = %d\n", counter);
    if(counter == 0) return 'N';

	for(int i = 0; i < LIBRARY_SIZE; i++){
		for (int n = 0; n < MAX_SIGNAL_LENGTH; n++){
	        for(int m = 0; m < counter; m++){
				float match;//  d[n-1][m-1]
				float del;//    d[n-1][m]
				float insert;// d[n][m-1]

				//prevent out of array
				if(n != 0 && m != 0) {//safe
					match = scoreMatrix[n-1][m-1];
					del = scoreMatrix[n-1][m];
					insert = scoreMatrix[n][m-1];
				}
				else if(n != 0) {// m is 0
					match = DTW_INF;
					del = scoreMatrix[n-1][m];
					insert = DTW_INF;
				}
				else if (m != 0 ){// n is 0
					match = DTW_INF;
					del = DTW_INF;
					insert = scoreMatrix[n][m-1];
				}
				else{match = del = insert = 0;}// all zero, special case return 0

				scoreMatrix[n][m] = euclidean_score_dtw(training_data[i][n], signal[m], NUM_IMU_DATA) 
				      + min_of_three( match, del, insert);
	        }
		}
        //check for intermediate result
        float score = DTW_INF;
        int idx = 0;
        for(idx; idx < MAX_SIGNAL_LENGTH; idx++){
            if(score > scoreMatrix[idx][counter-1]){
                score = scoreMatrix[idx][counter-1];
            }
        }
		printf("DTW for library %d (%c): Score = %f, idx = %d\n", i, gestures[i], score, idx);
		// if(score < thresholds[i])
		// {
        //     char str[64];
        //     sprintf(str, "Score: %4.2f", score);
        //     display_write(str, DISPLAY_LINE_1);
		// 	return gestures[i];
		// }
	}
	return 'N'; //no gesture satisfy
}

