#include "lib_gesture_flash.h"

const float thresholds[LIBRARY_SIZE] = {25,25,25, 25, 25, 25};

const char  gestures  [LIBRARY_SIZE] = "LLLFFF";


const float training_data[LIBRARY_SIZE][MAX_SIGNAL_LENGTH][NUM_IMU_DATA] = {
    {
        {-1.09, -1.41, -1.07, 44.49, -286.72, 61.38, -2.58, -3.16, -2.27, 1.91, 1.61, 1.53, 1.56},
{0.05, -0.89, 0.15, -17.60, 19.73, 9.70, 0.20, -2.11, 0.35, 1.94, 1.62, 1.47, 1.59},
{-0.05, -0.98, 0.40, 9.03, -6.55, 4.33, -0.11, -2.26, 0.93, 1.91, 1.62, 1.56, 1.60},
{-0.03, -0.96, 0.38, -4.03, 11.27, -0.13, -0.07, -2.21, 0.88, 1.92, 1.69, 1.52, 1.62},
{-0.01, -0.97, 0.35, 1.77, -5.64, -2.49, -0.03, -2.24, 0.81, 1.98, 1.65, 1.56, 1.62}
    },

    {
        {-0.72, -1.85, -0.77, 152.07, -286.72, 26.20, -1.34, -4.27, -1.96, 1.95, 1.65, 1.59, 1.53},
{-0.01, -1.10, 0.22, 41.95, 7.64, 29.58, -0.02, -2.53, 0.44, 1.89, 1.63, 1.48, 1.54},
{0.09, -0.97, 0.32, 6.76, -6.20, 9.11, 0.21, -2.25, 0.75, 1.96, 1.65, 1.60, 1.52},
{0.04, -1.01, 0.31, 4.24, -4.15, 3.55, 0.09, -2.32, 0.70, 1.94, 1.71, 1.47, 1.59},
{0.06, -0.96, 0.37, -4.08, 1.36, -6.37, 0.12, -2.20, 0.85, 1.93, 1.68, 1.50, 1.56}
    },

    {
        {-0.97, -2.00, -0.62, 205.60, -286.72, 79.72, -1.86, -4.53, -1.28, 1.96, 1.56, 1.59, 1.55},
{-0.03, -0.82, 0.35, 46.06, -0.69, 11.69, -0.06, -1.94, 0.72, 1.92, 1.62, 1.56, 1.56},
{0.02, -0.98, 0.30, 5.41, -1.02, 2.90, 0.05, -2.29, 0.67, 1.95, 1.65, 1.53, 1.57},
{0.00, -0.97, 0.30, -2.55, 17.97, -0.46, 0.01, -2.24, 0.70, 1.96, 1.65, 1.52, 1.60},
{0.04, -0.94, 0.37, 6.73, -19.30, 6.75, 0.09, -2.16, 0.86, 1.94, 1.74, 1.49, 1.53}
    },

    {
        {0.14, 0.39, 1.64, -77.94, -140.03, -33.70, 0.37, 0.99, 3.65, 1.76, 1.57, 1.54, 1.65},
{0.71, 0.16, 0.61, 9.04, 2.61, 5.68, 1.61, 0.39, 1.47, 1.67, 1.58, 1.58, 1.64},
{0.68, -0.01, 0.74, 3.28, -4.56, -2.72, 1.53, -0.01, 1.69, 1.70, 1.59, 1.61, 1.63},
{0.69, 0.02, 0.74, -0.57, 1.42, -2.02, 1.59, 0.05, 1.70, 1.72, 1.65, 1.56, 1.63},
{0.65, 0.04, 0.78, -2.51, 2.73, 1.70, 1.49, 0.09, 1.78, 1.71, 1.65, 1.61, 1.56}
    },

    {
{0.20, 0.15, 1.41, -119.76, -191.13, -24.35, 0.34, 0.41, 3.35, 1.76, 1.56, 1.63, 1.60},
{0.72, 0.08, 0.74, 18.76, 18.24, 5.48, 1.62, 0.15, 1.71, 1.65, 1.58, 1.52, 1.55},
{0.67, -0.03, 0.77, 5.81, 7.17, -4.40, 1.53, -0.08, 1.76, 1.69, 1.61, 1.59, 1.64},
{0.65, -0.03, 0.72, -1.35, 1.46, -0.04, 1.49, -0.08, 1.66, 1.72, 1.65, 1.56, 1.64},
{0.65, 0.01, 0.78, 0.18, -0.71, 0.52, 1.49, 0.03, 1.78, 1.66, 1.63, 1.62, 1.63}
    },

    {
{-0.33, 0.46, 2.00, -72.41, -159.58, -74.55, -0.70, 0.91, 4.35, 1.68, 1.61, 1.56, 1.60},
{0.70, 0.06, 0.79, -13.21, -9.11, -8.98, 1.61, 0.08, 1.81, 1.70, 1.63, 1.52, 1.64},
{0.69, -0.09, 0.74, 2.06, -0.95, -0.40, 1.60, -0.19, 1.71, 1.69, 1.65, 1.59, 1.55},
{0.69, -0.03, 0.70, -5.33, 5.29, 1.66, 1.56, -0.06, 1.60, 1.71, 1.67, 1.63, 1.56},
{0.69, -0.05, 0.73, 3.88, 1.01, -1.58, 1.58, -0.11, 1.67, 1.72, 1.60, 1.58, 1.69}
    }

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

