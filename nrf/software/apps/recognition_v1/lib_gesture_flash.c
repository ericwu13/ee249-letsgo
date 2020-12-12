#include "lib_gesture_flash.h"

const float thresholds[LIBRARY_SIZE] = {25,25,25,25,25,25,
										25,25,25,25,25,25,
										25,25,25,25,25,25,
										25,25,25,25,25,25};

const char  gestures  [LIBRARY_SIZE] = "FFBBLLRRHHGGHHUUSSSSSSSS";


const float training_data[LIBRARY_SIZE][MAX_SIGNAL_LENGTH][NUM_IMU_DATA] = {{
    {0.58, 0.63, -1.40, -21.36, -247.48, 17.78, 1.33, 1.57, -3.13, 1.95, 2.15, 2.21, 2.14},
    {0.60, 0.55, -0.55, -68.08, -286.72, -5.93, 1.32, 1.34, -1.26, 1.90, 2.03, 2.21, 2.15},
    {0.49, 0.48, 0.40, -4.13, -286.72, 16.37, 1.17, 1.09, 1.06, 1.92, 1.81, 2.06, 1.87},
    {-0.03, 0.65, 1.43, -72.24, -280.64, -5.76, -0.10, 1.58, 3.50, 1.78, 1.68, 1.75, 1.76},
    {-0.17, 0.31, 1.75, 40.94, -177.09, -19.95, -0.18, 0.69, 4.11, 1.69, 1.58, 1.60, 1.68},
    {-0.02, -0.00, 1.48, 14.64, -44.53, 37.21, 0.03, 0.05, 3.42, 1.62, 1.70, 1.58, 1.68},
    {0.22, 0.25, 1.22, -47.95, -1.13, 0.27, 0.50, 0.60, 2.79, 1.71, 1.66, 1.60, 1.67},
    {0.33, 0.34, 0.86, 28.48, 15.00, -5.01, 0.76, 0.79, 1.94, 1.68, 1.68, 1.69, 1.60},
    {0.40, 0.37, 0.74, 57.72, -23.30, -8.96, 0.95, 0.87, 1.70, 1.70, 1.61, 1.68, 1.66},
    {0.46, 0.33, 0.74, -2.01, 8.01, -0.36, 1.06, 0.74, 1.72, 1.75, 1.61, 1.65, 1.74},
    {0.43, 0.32, 0.89, -37.88, 21.85, 2.99, 1.01, 0.74, 2.04, 1.69, 1.62, 1.71, 1.64},
    {0.49, 0.39, 0.83, -14.23, 3.33, -6.00, 1.12, 0.90, 1.90, 1.71, 1.62, 1.73, 1.61},
    {0.44, 0.28, 0.84, 23.22, -13.38, -11.44, 1.01, 0.66, 1.92, 1.72, 1.69, 1.69, 1.63},
    {0.46, 0.33, 0.84, 3.13, -5.89, -0.18, 1.05, 0.76, 1.93, 1.67, 1.67, 1.71, 1.67},
    {0.43, 0.32, 0.87, -7.96, -1.32, -3.54, 0.99, 0.74, 1.98, 1.72, 1.63, 1.69, 1.65},
    {0.47, 0.31, 0.83, -10.61, 1.04, -1.23, 1.07, 0.70, 1.92, 1.69, 1.66, 1.71, 1.65},
    {0.46, 0.35, 0.84, -3.14, -4.34, -4.66, 1.05, 0.81, 1.92, 1.71, 1.70, 1.74, 1.74},
    {0.44, 0.33, 0.86, 0.94, -4.66, -3.92, 1.01, 0.76, 1.98, 1.64, 1.69, 1.67, 1.67},
    {0.44, 0.30, 0.81, -2.93, -3.93, -6.71, 1.01, 0.70, 1.84, 1.71, 1.70, 1.74, 1.74},
    {0.42, 0.25, 0.82, -0.27, -2.05, -1.41, 0.97, 0.59, 1.89, 1.66, 1.73, 1.62, 1.75}
},
{
    {0.67, 0.38, -0.99, -43.38, -237.47, 12.56, 1.55, 0.83, -2.32, 1.94, 2.11, 2.19, 2.12},
    {0.78, 0.55, -0.37, -28.12, -286.72, 19.78, 1.74, 1.31, -0.71, 1.92, 1.96, 2.13, 2.10},
    {0.55, 0.33, 0.41, 57.08, -286.72, 10.34, 1.25, 0.84, 0.98, 1.81, 1.84, 1.94, 1.81},
    {-0.35, 0.59, 1.66, -57.88, -226.84, -18.55, -0.88, 1.44, 3.87, 1.65, 1.67, 1.73, 1.68},
    {-0.10, 0.00, 1.74, 9.24, -128.87, 55.71, -0.15, 0.04, 4.20, 1.65, 1.59, 1.60, 1.63},
    {-0.07, 0.08, 1.55, -35.05, -41.74, -7.08, -0.03, 0.11, 3.52, 1.72, 1.67, 1.62, 1.64},
    {0.14, 0.38, 1.09, -57.15, 20.92, 8.22, 0.32, 0.86, 2.50, 1.66, 1.67, 1.68, 1.63},
    {0.24, 0.34, 0.89, 80.68, -30.83, 6.03, 0.54, 0.76, 2.07, 1.70, 1.66, 1.69, 1.69},
    {0.33, 0.41, 0.79, 67.72, -20.36, -13.55, 0.74, 0.93, 1.81, 1.74, 1.65, 1.62, 1.74},
    {0.31, 0.30, 0.88, -6.13, 12.15, -2.59, 0.70, 0.69, 2.02, 1.75, 1.62, 1.75, 1.63},
    {0.38, 0.32, 0.88, -62.60, 33.47, 0.31, 0.87, 0.72, 2.02, 1.68, 1.68, 1.64, 1.76},
    {0.37, 0.31, 0.93, -15.56, 7.96, -1.64, 0.83, 0.72, 2.14, 1.66, 1.69, 1.66, 1.71},
    {0.38, 0.36, 0.88, 11.44, -1.33, -3.39, 0.89, 0.82, 2.01, 1.67, 1.68, 1.67, 1.70},
    {0.37, 0.35, 0.85, 23.24, -4.96, -4.07, 0.84, 0.80, 1.94, 1.64, 1.75, 1.63, 1.76},
    {0.38, 0.35, 0.84, -3.86, -3.39, -1.34, 0.89, 0.79, 1.93, 1.69, 1.63, 1.64, 1.70},
    {0.37, 0.35, 0.90, -26.57, 15.26, -2.25, 0.85, 0.83, 2.06, 1.66, 1.72, 1.66, 1.73},
    {0.42, 0.31, 0.84, -3.26, 1.26, -5.77, 0.97, 0.70, 1.90, 1.76, 1.63, 1.79, 1.67},
    {0.38, 0.33, 0.86, 1.24, -2.20, -2.64, 0.89, 0.75, 1.98, 1.66, 1.71, 1.66, 1.77},
    {0.36, 0.34, 0.90, -0.53, 1.37, -3.18, 0.85, 0.80, 2.08, 1.76, 1.64, 1.78, 1.66},
    {0.43, 0.32, 0.81, -6.28, 8.40, -0.04, 0.98, 0.74, 1.86, 1.72, 1.71, 1.71, 1.65} 
},
{
    {-0.43, 0.00, -1.34, 127.39, -269.75, -286.72, -1.43, -0.96, -1.78, 1.95, 2.11, 2.19, 2.20},
    {0.07, -1.76, -0.50, 286.68, -286.72, -105.65, 0.27, -4.25, -1.29, 1.81, 1.99, 1.98, 1.93},
    {0.30, -2.00, -0.35, 286.68, -254.74, 246.91, 0.69, -4.59, -0.61, 1.74, 1.71, 1.72, 1.70},
    {0.67, -1.57, 0.27, 286.68, -176.36, 247.40, 1.85, -3.75, 0.70, 1.71, 1.62, 1.56, 1.54},
    {0.78, -0.71, 0.21, 286.68, -170.19, 232.46, 1.80, -1.60, 0.49, 1.63, 1.53, 1.52, 1.49},
    {0.53, -0.27, 1.18, 96.93, -100.64, 80.50, 1.26, -0.57, 2.63, 1.66, 1.55, 1.54, 1.56},
    {0.81, -0.17, 0.59, 37.28, 4.03, 35.58, 1.85, -0.42, 1.32, 1.65, 1.60, 1.53, 1.60},
    {0.95, -0.18, 0.55, 5.19, -8.03, 5.96, 2.19, -0.43, 1.26, 1.67, 1.53, 1.63, 1.49},
    {1.01, -0.14, 0.35, -14.92, -10.56, -0.21, 2.31, -0.31, 0.82, 1.64, 1.59, 1.57, 1.58},
    {0.95, -0.14, 0.32, -15.87, -2.62, 0.67, 2.20, -0.34, 0.73, 1.63, 1.60, 1.60, 1.50},
    {0.91, -0.13, 0.28, -14.50, -8.30, -0.60, 2.10, -0.29, 0.66, 1.69, 1.62, 1.56, 1.52},
    {0.85, -0.08, 0.32, -13.12, -1.93, -7.00, 1.93, -0.17, 0.73, 1.67, 1.59, 1.62, 1.60},
    {0.92, -0.14, 0.29, 0.22, 6.52, -1.52, 2.11, -0.32, 0.68, 1.73, 1.54, 1.65, 1.51},
    {0.90, -0.11, 0.34, -6.82, -0.73, -5.66, 2.07, -0.24, 0.77, 1.63, 1.59, 1.58, 1.56},
    {0.93, -0.13, 0.30, -1.45, 5.50, -3.49, 2.14, -0.28, 0.70, 1.65, 1.58, 1.65, 1.56},
    {0.96, -0.15, 0.30, -0.98, 7.45, -7.47, 2.21, -0.35, 0.69, 1.70, 1.58, 1.57, 1.51},
    {0.98, -0.17, 0.33, -5.70, 12.57, -7.33, 2.25, -0.41, 0.75, 1.65, 1.59, 1.65, 1.52},
    {0.96, -0.17, 0.32, 1.35, 4.68, 1.22, 2.20, -0.38, 0.73, 1.65, 1.61, 1.63, 1.52},
    {0.96, -0.15, 0.28, -9.09, -1.01, -1.81, 2.21, -0.35, 0.65, 1.70, 1.62, 1.60, 1.51},
    {0.92, -0.12, 0.32, -1.50, -0.82, -1.31, 2.11, -0.30, 0.73, 1.69, 1.60, 1.57, 1.61}
},
{
    {0.77, -0.36, -0.83, 57.72, -259.98, -161.95, 1.72, -0.85, -1.91, 1.91, 2.04, 1.99, 2.00},
    {0.60, -1.80, -0.40, 286.68, -286.72, 34.30, 1.42, -4.24, -0.89, 1.83, 1.88, 1.86, 1.77},
    {0.59, -2.00, -0.11, 286.68, -193.45, 234.16, 1.34, -4.59, -0.27, 1.71, 1.65, 1.63, 1.65},
    {0.15, -2.00, -0.24, 286.68, -238.21, 222.75, 0.20, -4.59, 0.36, 1.69, 1.62, 1.54, 1.54},
    {0.23, -1.25, 1.06, 286.68, -205.60, 284.93, 0.52, -2.66, 2.54, 1.65, 1.53, 1.58, 1.54},
    {0.45, -0.19, 0.95, 48.99, -38.50, 73.75, 1.01, -0.41, 2.16, 1.63, 1.58, 1.60, 1.50},
    {0.71, -0.23, 0.82, 32.65, 10.97, 9.14, 1.66, -0.51, 1.86, 1.69, 1.57, 1.53, 1.55},
    {1.01, -0.09, 0.23, -2.00, 18.25, 3.75, 2.32, -0.20, 0.53, 1.72, 1.61, 1.60, 1.56},
    {1.03, -0.15, 0.28, -22.50, 5.70, -9.55, 2.39, -0.35, 0.63, 1.64, 1.62, 1.51, 1.60},
    {0.98, -0.14, 0.26, -12.94, -9.56, -4.24, 2.24, -0.31, 0.62, 1.64, 1.55, 1.60, 1.58},
    {0.92, -0.15, 0.30, -22.23, -7.60, -10.40, 2.12, -0.35, 0.70, 1.75, 1.57, 1.52, 1.56},
    {0.89, -0.16, 0.37, -15.78, -12.79, -8.30, 2.02, -0.36, 0.87, 1.68, 1.59, 1.53, 1.57},
    {0.93, -0.15, 0.28, -12.40, 1.90, -5.66, 2.13, -0.35, 0.62, 1.72, 1.59, 1.53, 1.57},
    {0.91, -0.15, 0.31, -8.86, -0.62, -6.98, 2.10, -0.35, 0.73, 1.67, 1.60, 1.54, 1.57},
    {0.94, -0.16, 0.28, 1.61, -0.11, 2.48, 2.16, -0.36, 0.66, 1.70, 1.64, 1.59, 1.50},
    {0.93, -0.18, 0.31, -9.71, -3.01, -5.25, 2.14, -0.42, 0.70, 1.72, 1.62, 1.60, 1.53},
    {0.90, -0.15, 0.35, -2.98, -12.56, -0.52, 2.06, -0.34, 0.81, 1.65, 1.62, 1.60, 1.52},
    {0.95, -0.18, 0.33, -3.12, 3.46, -3.53, 2.19, -0.42, 0.74, 1.69, 1.54, 1.61, 1.60},
    {0.97, -0.20, 0.34, -1.15, -0.78, 0.17, 2.21, -0.45, 0.78, 1.70, 1.57, 1.62, 1.57},
    {0.95, -0.18, 0.34, -5.03, -1.75, 0.83, 2.17, -0.42, 0.80, 1.74, 1.61, 1.58, 1.53}
},
{
    {0.69, -0.40, -0.78, 81.17, -286.72, -158.85, 1.51, -1.10, -1.64, 1.90, 1.98, 1.76, 1.88},
    {-0.08, -1.04, 0.22, 278.38, -286.72, 72.23, -0.22, -2.65, 0.45, 1.87, 1.74, 1.65, 1.65},
    {0.23, -2.00, 0.60, 286.68, -286.72, 286.68, 0.48, -4.59, 1.29, 1.74, 1.55, 1.56, 1.50},
    {0.08, -2.00, 1.15, 248.61, -286.72, 286.68, 0.32, -4.59, 2.68, 1.71, 1.48, 1.60, 1.52},
    {-0.06, -1.01, 0.73, 216.43, -286.72, 286.68, -0.32, -2.51, 1.58, 1.66, 1.58, 1.59, 1.48},
    {-0.66, -0.90, 1.37, 108.67, -286.72, 242.22, -1.48, -1.90, 3.16, 1.63, 1.48, 1.64, 1.47},
    {-0.60, -0.40, 1.00, -29.23, -154.16, 25.09, -1.40, -0.98, 2.25, 1.63, 1.53, 1.63, 1.52},
    {-0.34, -0.54, 0.96, 20.84, -58.20, 33.01, -0.74, -1.31, 2.22, 1.66, 1.55, 1.71, 1.47},
    {-0.07, -0.67, 0.81, 8.65, -6.55, 13.96, -0.15, -1.56, 1.86, 1.66, 1.55, 1.66, 1.51},
    {0.08, -0.68, 0.80, 3.35, -4.28, 6.92, 0.18, -1.57, 1.83, 1.73, 1.49, 1.69, 1.54},
    {0.09, -0.65, 0.80, 5.21, 2.12, 8.56, 0.19, -1.50, 1.83, 1.64, 1.54, 1.67, 1.53},
    {0.08, -0.61, 0.78, -4.38, -2.10, -7.13, 0.21, -1.41, 1.82, 1.63, 1.58, 1.68, 1.48},
    {0.10, -0.67, 0.80, 0.61, -0.76, -0.74, 0.23, -1.54, 1.83, 1.73, 1.54, 1.69, 1.47},
    {0.13, -0.69, 0.82, -1.23, 0.71, 0.97, 0.29, -1.58, 1.88, 1.64, 1.53, 1.72, 1.48},
    {0.09, -0.64, 0.79, -3.43, 0.35, -0.45, 0.21, -1.46, 1.80, 1.67, 1.55, 1.65, 1.52},
    {0.09, -0.66, 0.80, -0.25, -3.29, 0.24, 0.21, -1.52, 1.82, 1.66, 1.53, 1.67, 1.53},
    {0.07, -0.63, 0.79, -2.16, 0.31, -6.06, 0.16, -1.46, 1.81, 1.65, 1.59, 1.65, 1.56},
    {0.10, -0.66, 0.79, -7.83, -6.39, -10.59, 0.23, -1.52, 1.80, 1.71, 1.54, 1.69, 1.54},
    {0.11, -0.66, 0.77, -0.25, 6.12, 0.60, 0.25, -1.51, 1.76, 1.68, 1.52, 1.71, 1.50},
    {0.08, -0.64, 0.78, -2.85, -1.23, -2.39, 0.18, -1.45, 1.79, 1.68, 1.55, 1.65, 1.53}
},
{
    {-0.38, -1.66, 1.18, 140.33, -286.72, 286.68, -0.82, -3.21, 2.13, 1.72, 1.48, 1.61, 1.47},
    {-0.53, -0.95, 1.21, 94.04, -211.28, 286.68, -1.30, -2.25, 2.73, 1.69, 1.58, 1.52, 1.54},
    {-0.60, -0.51, 1.07, 21.28, -162.65, 162.31, -1.46, -1.13, 2.44, 1.71, 1.56, 1.58, 1.51},
    {-0.37, -0.36, 0.98, -33.01, -73.85, 37.48, -0.83, -0.82, 2.20, 1.65, 1.52, 1.62, 1.53},
    {-0.23, -0.46, 0.88, 19.81, -38.22, 51.80, -0.52, -1.08, 2.00, 1.65, 1.50, 1.61, 1.53},
    {-0.08, -0.52, 0.88, -5.34, -23.52, 5.22, -0.20, -1.19, 2.01, 1.69, 1.53, 1.58, 1.48},
    {-0.02, -0.54, 0.86, -4.54, -14.39, -3.68, -0.05, -1.21, 1.95, 1.68, 1.50, 1.57, 1.50},
    {0.05, -0.60, 0.88, 2.56, 6.11, 4.84, 0.07, -1.36, 2.00, 1.73, 1.56, 1.66, 1.54},
    {0.07, -0.55, 0.85, -5.85, 3.85, -3.38, 0.14, -1.25, 1.95, 1.67, 1.56, 1.65, 1.48},
    {0.05, -0.55, 0.84, -1.55, 9.57, -5.53, 0.12, -1.25, 1.91, 1.65, 1.52, 1.67, 1.54},
    {0.07, -0.56, 0.82, -2.69, 1.58, -10.41, 0.15, -1.29, 1.89, 1.73, 1.51, 1.64, 1.48},
    {0.11, -0.66, 0.82, 0.67, 4.55, -1.33, 0.24, -1.50, 1.90, 1.75, 1.58, 1.67, 1.55},
    {0.05, -0.59, 0.85, -6.13, -2.90, -4.79, 0.11, -1.37, 1.95, 1.70, 1.58, 1.66, 1.55},
    {0.07, -0.59, 0.85, -0.66, 6.52, 2.52, 0.16, -1.36, 1.95, 1.73, 1.49, 1.71, 1.48},
    {0.07, -0.56, 0.82, -5.29, 1.87, -4.62, 0.18, -1.28, 1.90, 1.68, 1.57, 1.61, 1.54},
    {0.06, -0.57, 0.82, 0.23, 3.05, -1.71, 0.16, -1.23, 1.90, 1.72, 1.51, 1.67, 1.52},
    {0.07, -0.58, 0.84, -0.69, -1.64, -2.70, 0.15, -1.34, 1.94, 1.74, 1.52, 1.66, 1.51},
    {0.07, -0.58, 0.84, 0.21, 3.38, -1.60, 0.16, -1.33, 1.94, 1.75, 1.58, 1.66, 1.55},
    {0.07, -0.59, 0.84, -2.11, 0.50, -3.48, 0.16, -1.36, 1.92, 1.73, 1.56, 1.65, 1.53},
    {0.08, -0.59, 0.84, -1.21, 4.03, -0.75, 0.18, -1.35, 1.93, 1.70, 1.51, 1.66, 1.54}
},
{
    {-0.95, -0.52, 1.83, 121.28, 15.44, -286.72, -2.11, -1.11, 4.05, 1.73, 1.53, 1.68, 1.55},
    {-0.85, -1.00, 1.18, 46.74, 286.68, -286.72, -1.82, -2.44, 2.65, 1.75, 1.48, 1.72, 1.53},
    {-0.37, -1.49, 0.52, 72.72, 286.68, -286.72, -0.62, -3.72, 1.24, 1.78, 1.52, 1.71, 1.58},
    {-0.06, -1.45, 0.12, 3.26, 212.60, -129.82, 0.24, -3.78, 0.69, 1.76, 1.60, 1.74, 1.61},
    {0.15, -0.99, 0.38, -3.95, 79.97, -6.76, 0.41, -2.20, 0.93, 1.78, 1.60, 1.76, 1.60},
    {0.24, -0.90, 0.55, 10.74, 13.13, 8.27, 0.56, -2.06, 1.26, 1.78, 1.54, 1.74, 1.61},
    {0.25, -0.83, 0.57, 5.22, 5.07, 15.64, 0.57, -1.97, 1.31, 1.77, 1.55, 1.74, 1.60},
    {0.28, -0.76, 0.60, 4.43, 4.93, 16.16, 0.61, -1.75, 1.39, 1.73, 1.58, 1.77, 1.55},
    {0.23, -0.75, 0.63, 8.51, 2.28, -0.26, 0.53, -1.72, 1.43, 1.72, 1.59, 1.67, 1.60},
    {0.22, -0.79, 0.64, -2.00, 4.48, 6.36, 0.51, -1.81, 1.46, 1.75, 1.62, 1.72, 1.58},
    {0.25, -0.75, 0.65, 2.90, 8.30, 1.75, 0.58, -1.73, 1.49, 1.72, 1.60, 1.76, 1.56},
    {0.27, -0.76, 0.62, -1.88, 2.14, 7.74, 0.63, -1.74, 1.41, 1.78, 1.61, 1.70, 1.61},
    {0.26, -0.74, 0.63, 1.74, 6.16, -5.17, 0.61, -1.71, 1.43, 1.80, 1.54, 1.72, 1.64},
    {0.25, -0.79, 0.61, -4.56, 5.33, 2.67, 0.56, -1.82, 1.41, 1.79, 1.58, 1.77, 1.62},
    {0.28, -0.76, 0.60, -3.59, 6.68, 2.57, 0.63, -1.74, 1.39, 1.80, 1.60, 1.69, 1.62},
    {0.26, -0.79, 0.61, 1.78, 3.38, -5.93, 0.59, -1.83, 1.40, 1.74, 1.60, 1.78, 1.59},
    {0.26, -0.81, 0.60, -1.96, 2.48, -0.74, 0.59, -1.86, 1.39, 1.75, 1.62, 1.74, 1.58},
    {0.25, -0.80, 0.61, 1.84, -1.80, -0.20, 0.57, -1.84, 1.39, 1.74, 1.59, 1.74, 1.60},
    {0.26, -0.80, 0.64, -2.94, 2.59, 2.34, 0.61, -1.85, 1.48, 1.79, 1.58, 1.75, 1.62},
    {0.29, -0.76, 0.62, -9.80, 7.39, 9.46, 0.66, -1.74, 1.42, 1.80, 1.63, 1.74, 1.63}
},
{
    {-1.02, -1.53, 1.39, -159.38, 235.00, -286.72, -2.34, -3.21, 3.26, 1.71, 1.53, 1.69, 1.59},
    {-0.21, -1.09, 0.39, -225.19, 286.68, -286.72, -0.47, -2.28, 0.88, 1.75, 1.54, 1.68, 1.53},
    {-0.25, -1.67, 0.77, -53.76, 286.68, -210.31, -0.42, -3.73, 1.72, 1.70, 1.55, 1.70, 1.54},
    {0.30, -0.96, 0.19, -21.06, 180.65, -24.25, 0.68, -2.07, 0.50, 1.76, 1.53, 1.70, 1.59},
    {0.40, -0.82, 0.57, 10.18, 43.56, -3.61, 0.88, -2.06, 1.30, 1.76, 1.54, 1.69, 1.59},
    {0.43, -0.74, 0.69, -1.26, 23.17, 8.69, 0.95, -1.68, 1.56, 1.70, 1.59, 1.69, 1.57},
    {0.35, -0.73, 0.70, 16.99, 1.60, 5.84, 0.76, -1.71, 1.63, 1.79, 1.56, 1.67, 1.60},
    {0.43, -0.73, 0.62, 4.21, 9.21, 7.38, 1.00, -1.65, 1.42, 1.74, 1.60, 1.73, 1.58},
    {0.36, -0.68, 0.67, 10.06, 5.87, 2.98, 0.82, -1.60, 1.56, 1.74, 1.59, 1.64, 1.59},
    {0.39, -0.65, 0.65, -0.09, 3.51, 9.16, 0.89, -1.50, 1.45, 1.79, 1.55, 1.69, 1.62},
    {0.34, -0.63, 0.70, 4.40, -3.78, -0.49, 0.78, -1.45, 1.61, 1.72, 1.61, 1.72, 1.56},
    {0.37, -0.63, 0.69, 0.99, 9.39, -0.99, 0.88, -1.46, 1.56, 1.76, 1.58, 1.72, 1.63},
    {0.34, -0.66, 0.66, 10.24, -4.02, 0.25, 0.77, -1.52, 1.53, 1.79, 1.62, 1.68, 1.60},
    {0.37, -0.69, 0.66, 1.16, 7.04, -2.55, 0.86, -1.59, 1.49, 1.78, 1.62, 1.74, 1.63},
    {0.35, -0.66, 0.65, 0.19, -3.80, 2.30, 0.80, -1.51, 1.49, 1.73, 1.61, 1.69, 1.64},
    {0.38, -0.74, 0.68, 3.62, 1.26, -4.20, 0.83, -1.69, 1.55, 1.82, 1.55, 1.73, 1.60},
    {0.36, -0.68, 0.66, -3.52, 0.79, 3.59, 0.83, -1.57, 1.51, 1.78, 1.56, 1.74, 1.61},
    {0.37, -0.72, 0.66, 4.93, -0.46, 0.07, 0.85, -1.66, 1.51, 1.80, 1.58, 1.69, 1.63},
    {0.36, -0.70, 0.66, -0.52, -1.98, 4.29, 0.83, -1.58, 1.53, 1.79, 1.56, 1.71, 1.59},
    {0.38, -0.69, 0.67, 3.46, 2.90, 0.43, 0.88, -1.58, 1.54, 1.75, 1.61, 1.69, 1.64}
},
{
    {0.83, 0.10, -1.32, -50.90, -286.72, -24.74, 1.82, 0.23, -2.86, 1.89, 2.08, 2.13, 2.08},
    {-0.87, -0.09, -0.16, -121.70, -286.72, 94.25, -2.29, -0.17, -0.23, 1.70, 1.92, 2.04, 1.85},
    {-0.92, 0.12, 0.71, -160.76, -286.72, 214.35, -2.00, 0.31, 1.59, 1.58, 1.68, 1.80, 1.74},
    {-1.06, 1.24, 1.59, -241.29, -286.72, 105.65, -3.33, 3.12, 4.19, 1.64, 1.56, 1.67, 1.52},
    {-0.89, 0.67, 1.53, -36.23, 35.67, 100.56, -1.87, 1.12, 3.55, 1.55, 1.61, 1.58, 1.59},
    {-0.43, 0.71, 1.27, -14.09, -41.98, 2.43, -1.05, 1.61, 2.90, 1.58, 1.60, 1.62, 1.54},
    {-0.40, 0.43, 1.20, 41.43, 5.43, 6.74, -0.89, 0.98, 2.71, 1.57, 1.63, 1.63, 1.53},
    {-0.28, 0.46, 1.04, 10.25, 15.35, -7.51, -0.65, 1.05, 2.38, 1.56, 1.62, 1.59, 1.60},
    {-0.29, 0.45, 1.01, 11.26, 13.92, -1.46, -0.66, 1.04, 2.33, 1.57, 1.65, 1.60, 1.62},
    {-0.28, 0.44, 0.96, -5.23, 34.48, -20.54, -0.64, 1.01, 2.17, 1.59, 1.59, 1.68, 1.59},
    {-0.24, 0.40, 0.86, 2.18, 14.58, -1.01, -0.58, 0.93, 1.94, 1.61, 1.60, 1.70, 1.60},
    {-0.24, 0.34, 0.83, -8.43, 27.17, -15.09, -0.55, 0.78, 1.90, 1.64, 1.59, 1.71, 1.55},
    {-0.19, 0.38, 0.80, 0.88, 2.79, 2.87, -0.44, 0.88, 1.87, 1.63, 1.66, 1.68, 1.63},
    {-0.21, 0.37, 0.88, -3.72, 16.99, -10.35, -0.48, 0.85, 2.03, 1.63, 1.65, 1.63, 1.58},
    {-0.20, 0.36, 0.90, 3.28, 3.28, -2.90, -0.48, 0.84, 2.07, 1.58, 1.65, 1.63, 1.62},
    {-0.19, 0.37, 0.91, 3.62, 15.92, -8.41, -0.43, 0.84, 2.07, 1.59, 1.60, 1.71, 1.56},
    {-0.17, 0.37, 0.93, 2.69, 5.28, -1.85, -0.43, 0.85, 2.14, 1.62, 1.66, 1.70, 1.58},
    {-0.18, 0.40, 0.91, 0.83, 13.68, -4.13, -0.41, 0.93, 2.09, 1.59, 1.66, 1.69, 1.59},
    {-0.14, 0.38, 0.88, 0.52, 9.19, -4.53, -0.31, 0.87, 2.01, 1.58, 1.61, 1.68, 1.65},
    {-0.16, 0.38, 0.92, 5.22, 4.25, -2.20, -0.36, 0.87, 2.08, 1.62, 1.61, 1.68, 1.62}
},
{
    {1.08, -0.47, -1.37, 2.00, -286.72, 115.09, 2.50, -1.11, -3.11, 1.81, 1.97, 1.89, 1.84},
    {-1.37, -0.55, -0.32, -6.21, -286.72, 266.56, -3.15, -1.07, -0.60, 1.75, 1.68, 1.77, 1.75},
    {-0.89, 0.05, 0.93, -52.59, -286.72, 181.24, -2.35, 0.02, 2.03, 1.63, 1.63, 1.61, 1.55},
    {-0.99, 0.81, 1.59, 32.11, -248.95, 59.27, -2.49, 2.40, 3.92, 1.60, 1.61, 1.59, 1.54},
    {-0.48, 0.49, 1.51, 93.95, 58.88, 44.91, -1.51, 0.82, 3.75, 1.64, 1.62, 1.58, 1.55},
    {-0.34, 0.34, 1.40, 31.60, -69.99, 21.17, -0.67, 0.77, 3.23, 1.57, 1.65, 1.54, 1.62},
    {-0.20, 0.31, 1.19, 1.76, 10.23, -0.97, -0.47, 0.72, 2.68, 1.62, 1.60, 1.59, 1.55},
    {-0.20, 0.31, 1.11, 5.76, -17.03, 2.97, -0.46, 0.71, 2.54, 1.62, 1.64, 1.61, 1.58},
    {-0.23, 0.29, 1.04, -6.89, 14.85, -10.14, -0.50, 0.66, 2.38, 1.63, 1.65, 1.62, 1.61},
    {-0.20, 0.27, 0.95, -1.34, 2.09, -1.85, -0.46, 0.61, 2.19, 1.59, 1.59, 1.57, 1.59},
    {-0.23, 0.19, 0.92, -3.18, 8.46, -7.08, -0.51, 0.44, 2.11, 1.65, 1.60, 1.58, 1.63},
    {-0.20, 0.25, 0.90, -3.09, 1.58, 0.43, -0.46, 0.56, 2.07, 1.59, 1.65, 1.60, 1.61},
    {-0.20, 0.19, 0.93, -3.68, 0.30, -3.06, -0.47, 0.45, 2.14, 1.60, 1.67, 1.63, 1.55},
    {-0.20, 0.22, 0.94, -1.72, -0.20, -0.80, -0.43, 0.52, 2.16, 1.58, 1.68, 1.56, 1.65},
    {-0.22, 0.21, 0.98, -2.88, -3.03, -1.40, -0.50, 0.48, 2.24, 1.63, 1.62, 1.62, 1.59},
    {-0.21, 0.24, 0.99, 2.46, 4.24, -2.49, -0.50, 0.55, 2.26, 1.58, 1.68, 1.56, 1.67},
    {-0.20, 0.24, 0.96, -1.46, 1.18, -0.38, -0.45, 0.55, 2.20, 1.62, 1.62, 1.63, 1.56},
    {-0.19, 0.25, 0.97, -2.15, -0.79, 0.20, -0.47, 0.57, 2.23, 1.65, 1.66, 1.64, 1.65},
    {-0.21, 0.26, 0.97, -3.53, 1.11, -2.42, -0.48, 0.61, 2.22, 1.63, 1.62, 1.58, 1.63},
    {-0.20, 0.24, 0.95, -2.64, 1.50, -3.04, -0.47, 0.56, 2.20, 1.67, 1.62, 1.60, 1.66}
},
{
    {-0.21, 0.55, 1.47, 120.32, -3.57, -14.32, -0.57, 1.15, 3.24, 1.89, 2.18, 2.11, 2.10},
    {-0.07, 0.08, 1.32, 166.63, -31.40, 58.19, 0.03, 0.45, 3.10, 1.95, 2.18, 2.18, 2.14},
    {0.51, 0.13, 0.72, 178.61, 113.37, -44.52, 1.23, 0.03, 1.58, 1.96, 2.31, 2.10, 2.13},
    {0.34, 0.07, 1.36, 77.14, 65.73, -19.00, 0.84, 0.17, 2.99, 2.05, 2.34, 2.19, 2.06},
    {0.68, -0.13, 1.11, 35.86, 36.54, -12.50, 1.53, -0.30, 2.53, 2.16, 2.26, 2.08, 1.99},
    {0.55, -0.10, 1.17, 0.42, 31.67, -4.69, 1.25, -0.22, 2.72, 2.09, 2.14, 2.16, 2.00},
    {0.60, -0.09, 0.99, -7.15, 37.28, -2.18, 1.35, -0.21, 2.29, 2.03, 2.11, 2.12, 2.14},
    {0.44, -0.15, 0.87, -2.18, 44.14, -5.70, 1.03, -0.34, 1.98, 2.00, 2.27, 2.11, 2.03},
    {0.37, -0.18, 0.76, -4.21, 23.22, -0.99, 0.86, -0.42, 1.75, 2.03, 2.19, 2.20, 1.98},
    {0.45, -0.17, 0.70, -1.00, 18.54, -2.10, 1.04, -0.36, 1.60, 2.02, 2.26, 2.09, 2.08},
    {0.43, -0.11, 0.79, 0.92, -0.28, -0.96, 1.01, -0.25, 1.81, 2.12, 2.28, 2.20, 2.11},
    {0.50, -0.17, 0.81, 8.70, 5.57, -3.79, 1.16, -0.40, 1.85, 2.04, 2.20, 2.14, 2.04},
    {0.48, -0.15, 0.90, 4.24, -3.27, -6.69, 1.09, -0.36, 2.06, 2.09, 2.23, 2.10, 1.99},
    {0.58, -0.21, 0.85, 3.34, 2.62, -4.97, 1.33, -0.47, 1.94, 1.99, 2.21, 2.17, 1.97},
    {0.49, -0.19, 0.95, 1.62, -2.51, -1.82, 1.13, -0.46, 2.18, 2.01, 2.30, 2.06, 2.09},
    {0.57, -0.15, 0.89, -0.46, 1.63, 0.59, 1.29, -0.36, 2.06, 2.00, 2.23, 2.11, 2.05},
    {0.52, -0.17, 0.90, -1.30, 10.67, 0.38, 1.20, -0.41, 2.06, 1.98, 2.31, 2.06, 2.13},
    {0.52, -0.17, 0.84, -0.97, 7.51, 0.33, 1.19, -0.38, 1.92, 1.98, 2.29, 2.11, 1.98},
    {0.50, -0.17, 0.81, -1.30, 3.39, 0.41, 1.13, -0.38, 1.88, 2.05, 2.20, 2.13, 2.04},
    {0.49, -0.19, 0.82, 0.81, 1.52, 0.67, 1.13, -0.44, 1.87, 2.08, 2.14, 2.18, 1.98}
},
{
    {0.24, -0.09, -0.43, -73.57, -286.72, 196.18, 0.38, -0.20, -1.01, 1.69, 1.61, 1.67, 1.59},
    {-0.10, -0.44, -0.66, -11.97, -286.72, 286.68, -0.39, -0.85, -1.51, 1.73, 1.50, 1.64, 1.54},
    {-0.79, 0.30, 0.15, 192.45, -277.26, 22.95, -1.72, 0.68, 0.53, 1.64, 1.57, 1.54, 1.64},
    {-0.54, 0.56, 1.10, -47.99, -95.29, -18.07, -1.19, 1.33, 2.74, 1.66, 1.75, 1.78, 1.69},
    {-0.18, 0.91, 1.76, 148.79, 25.98, 9.56, -0.24, 2.14, 4.04, 1.65, 1.98, 1.93, 1.93},
    {0.62, 0.21, 0.72, 167.54, 84.00, 32.96, 1.65, 0.34, 1.86, 1.81, 2.27, 2.11, 2.10},
    {0.07, 0.07, 1.58, 171.13, 125.61, -8.12, 0.12, 0.19, 3.50, 1.99, 2.15, 2.08, 2.17},
    {0.80, 0.25, 1.09, 31.86, 35.72, 28.33, 1.77, 0.54, 2.52, 1.96, 2.29, 2.08, 2.08},
    {0.78, -0.05, 1.01, 8.51, 117.99, 10.76, 1.84, -0.03, 2.27, 1.97, 2.21, 2.20, 1.97},
    {0.69, -0.02, 0.98, 16.09, 64.57, -1.16, 1.57, -0.04, 2.24, 2.11, 2.18, 2.08, 2.10},
    {0.81, -0.11, 0.56, -8.05, 43.39, -8.58, 1.88, -0.32, 1.26, 2.09, 2.13, 2.12, 1.99},
    {0.47, -0.14, 0.64, -0.34, 37.64, -1.82, 1.09, -0.31, 1.41, 2.00, 2.28, 2.04, 2.11},
    {0.63, -0.13, 0.54, -5.43, 2.92, 0.47, 1.44, -0.26, 1.27, 1.99, 2.11, 2.07, 2.04},
    {0.57, -0.12, 0.71, 2.13, 9.79, 1.37, 1.33, -0.25, 1.62, 2.12, 2.10, 2.16, 1.98},
    {0.65, -0.10, 0.75, 8.56, -2.62, -6.74, 1.47, -0.26, 1.73, 2.00, 2.11, 2.14, 2.05},
    {0.63, -0.13, 0.79, 7.32, 7.84, -4.57, 1.50, -0.32, 1.84, 2.08, 2.24, 2.16, 2.07},
    {0.71, -0.12, 0.75, 4.49, -7.80, -4.97, 1.59, -0.27, 1.74, 1.99, 2.23, 2.15, 2.03},
    {0.71, -0.13, 0.78, 3.68, 0.05, -1.81, 1.61, -0.30, 1.80, 1.99, 2.10, 2.07, 1.99},
    {0.68, -0.11, 0.79, -3.93, 3.64, 0.85, 1.56, -0.23, 1.82, 1.98, 2.11, 2.05, 1.98},
    {0.66, -0.14, 0.79, 0.70, 8.94, 0.26, 1.52, -0.33, 1.80, 2.03, 2.09, 2.06, 2.08}

},
{
    {0.41, 0.17, -0.72, -96.72, -286.72, -112.39, 0.88, 0.33, -1.44, 1.76, 2.04, 2.26, 2.02},
    {0.19, -0.17, -0.56, -53.07, -286.72, 34.97, 0.36, -0.38, -1.30, 1.74, 1.95, 2.11, 2.02},
    {-0.65, -0.35, 0.52, -55.82, -286.72, 113.27, -1.28, -0.74, 1.17, 1.68, 1.78, 2.00, 1.81},
    {-0.50, 0.36, 0.75, -19.06, -286.72, 154.99, -1.11, 0.71, 1.66, 1.67, 1.64, 1.66, 1.56},
    {-0.73, 0.30, 1.41, 30.09, -220.40, 71.34, -1.56, 0.64, 3.21, 1.59, 1.55, 1.64, 1.52},
    {-0.66, 0.35, 1.17, 6.52, -88.52, 56.81, -1.58, 0.72, 2.64, 1.65, 1.56, 1.56, 1.54},
    {-0.59, 0.30, 1.31, 105.31, -0.65, 26.86, -1.33, 0.72, 2.96, 1.60, 1.55, 1.62, 1.63},
    {-0.49, 0.11, 1.36, 77.99, 28.59, -7.91, -1.16, 0.28, 3.06, 1.61, 1.58, 1.62, 1.59},
    {-0.32, 0.19, 1.27, 16.49, -9.14, 5.07, -0.81, 0.40, 2.92, 1.67, 1.57, 1.64, 1.58},
    {-0.35, 0.14, 1.20, 1.91, 1.62, 1.64, -0.78, 0.31, 2.76, 1.57, 1.61, 1.62, 1.57},
    {-0.33, 0.14, 1.15, -1.41, 20.88, -5.65, -0.77, 0.34, 2.62, 1.63, 1.60, 1.60, 1.63},
    {-0.33, 0.14, 1.01, -25.31, 16.35, -2.25, -0.76, 0.34, 2.29, 1.57, 1.65, 1.58, 1.66},
    {-0.31, 0.09, 0.89, -6.76, 18.02, -8.48, -0.71, 0.19, 2.01, 1.63, 1.58, 1.64, 1.59},
    {-0.31, 0.10, 0.85, -12.37, 0.77, -3.08, -0.70, 0.22, 1.95, 1.58, 1.61, 1.67, 1.57},
    {-0.30, 0.09, 0.88, -10.03, 6.41, -2.93, -0.69, 0.21, 2.03, 1.63, 1.64, 1.64, 1.57},
    {-0.28, 0.12, 0.90, -1.18, -1.59, 3.12, -0.64, 0.28, 2.08, 1.58, 1.66, 1.60, 1.67},
    {-0.29, 0.14, 0.97, -2.89, -1.05, -0.91, -0.66, 0.32, 2.24, 1.63, 1.59, 1.67, 1.59},
    {-0.31, 0.12, 0.99, -0.11, -2.30, -0.05, -0.72, 0.29, 2.28, 1.61, 1.58, 1.62, 1.59},
    {-0.31, 0.13, 1.00, -3.86, 3.74, -2.39, -0.70, 0.31, 2.28, 1.60, 1.61, 1.61, 1.65},
    {-0.30, 0.13, 0.97, -1.84, 7.53, -2.05, -0.69, 0.30, 2.21, 1.64, 1.62, 1.68, 1.59}
},
{
    {1.10, -0.19, -0.83, -50.79, -286.72, 80.60, 2.56, -0.39, -1.94, 1.79, 1.97, 1.97, 1.87},
    {-0.69, 0.00, 0.07, 86.35, -286.72, 110.16, -1.35, 0.09, 0.25, 1.76, 1.74, 1.80, 1.65},
    {-0.13, -0.23, 0.58, 41.63, -286.72, 115.82, -0.45, -0.73, 1.19, 1.70, 1.71, 1.71, 1.59},
    {-0.63, 0.38, 0.70, -21.55, -286.72, 105.67, -1.35, 0.91, 1.70, 1.67, 1.64, 1.55, 1.61},
    {-0.56, -0.04, 1.11, 4.38, -225.29, 50.99, -1.19, 0.04, 2.61, 1.65, 1.59, 1.55, 1.63},
    {-0.66, 0.55, 1.30, 9.66, -145.74, 56.09, -1.39, 1.08, 3.07, 1.59, 1.64, 1.53, 1.65},
    {-0.49, 0.27, 1.44, 75.71, 20.06, 15.15, -1.10, 0.65, 3.30, 1.61, 1.56, 1.56, 1.57},
    {-0.39, 0.18, 1.50, 41.02, -3.49, 2.19, -0.85, 0.41, 3.42, 1.62, 1.60, 1.62, 1.61},
    {-0.20, 0.15, 1.29, -5.97, 15.63, -3.99, -0.49, 0.38, 2.92, 1.65, 1.55, 1.58, 1.59},
    {-0.19, 0.21, 1.11, 10.51, -3.45, -0.64, -0.44, 0.47, 2.55, 1.60, 1.60, 1.60, 1.56},
    {-0.22, 0.13, 1.00, -13.74, 28.50, -13.06, -0.52, 0.32, 2.28, 1.60, 1.65, 1.56, 1.66},
    {-0.23, 0.08, 0.93, -5.95, 3.30, -11.26, -0.54, 0.16, 2.14, 1.67, 1.59, 1.60, 1.62},
    {-0.21, 0.07, 0.90, -4.53, 7.93, -5.86, -0.49, 0.15, 2.11, 1.65, 1.56, 1.66, 1.59},
    {-0.23, 0.07, 0.96, -5.85, 2.11, -3.88, -0.52, 0.17, 2.19, 1.68, 1.56, 1.67, 1.53},
    {-0.20, 0.09, 0.93, 1.24, 2.07, 2.62, -0.46, 0.25, 2.15, 1.61, 1.58, 1.60, 1.58},
    {-0.22, 0.10, 0.99, -3.36, 2.17, -2.56, -0.51, 0.23, 2.28, 1.67, 1.60, 1.60, 1.63},
    {-0.20, 0.12, 1.01, -0.16, 5.19, -4.60, -0.47, 0.27, 2.31, 1.68, 1.63, 1.66, 1.64},
    {-0.19, 0.12, 1.00, -0.70, 4.73, -4.65, -0.44, 0.28, 2.31, 1.65, 1.62, 1.66, 1.66},
    {-0.20, 0.13, 1.00, -2.79, 5.49, -2.88, -0.47, 0.31, 2.28, 1.67, 1.59, 1.60, 1.59},
    {-0.20, 0.10, 1.00, -4.29, 5.91, -7.54, -0.47, 0.23, 2.29, 1.68, 1.60, 1.59, 1.66}
},
{
    {0.30, -0.13, 1.76, -35.27, 39.14, 10.05, 0.72, -0.36, 4.23, 2.00, 2.20, 2.26, 2.06},
    {0.15, -0.30, 2.00, -45.57, 158.35, 13.98, 0.38, -0.57, 4.59, 1.95, 2.23, 2.19, 2.14},
    {0.36, 0.24, 0.28, -36.30, 196.81, 14.42, 0.87, 0.53, 0.48, 2.00, 2.18, 2.26, 2.11},
    {0.51, 0.35, -0.59, -44.66, 151.76, 7.45, 1.25, 0.75, -1.40, 1.94, 2.24, 2.26, 2.13},
    {0.73, 0.08, -0.07, -76.22, 91.44, 19.64, 1.62, 0.21, -0.05, 1.89, 2.20, 2.19, 2.13},
    {1.03, 0.31, -0.07, -286.72, 9.46, 68.27, 2.55, 0.84, -0.26, 1.79, 1.89, 2.04, 1.98},
    {0.85, 0.83, 0.28, -37.76, 73.39, -76.21, 1.76, 1.85, 0.67, 1.73, 1.63, 1.90, 1.81},
    {1.30, 0.07, -0.10, -3.84, 35.19, 37.23, 3.01, 0.23, -0.29, 1.65, 1.65, 1.80, 1.77},
    {0.80, 0.38, 0.05, 36.72, 59.06, -3.99, 1.88, 0.84, 0.09, 1.66, 1.56, 1.69, 1.68},
    {0.92, 0.36, 0.07, -2.72, 58.75, -14.08, 2.09, 0.82, 0.22, 1.69, 1.51, 1.65, 1.61},
    {0.98, 0.44, 0.01, -13.19, 72.63, -17.83, 2.27, 0.99, 0.05, 1.69, 1.58, 1.59, 1.55},
    {1.02, 0.30, -0.14, -2.77, 20.59, -14.69, 2.38, 0.62, -0.38, 1.63, 1.58, 1.65, 1.51},
    {0.87, 0.29, -0.18, 5.05, -2.48, -4.11, 2.01, 0.62, -0.40, 1.69, 1.52, 1.64, 1.56},
    {0.85, 0.31, -0.00, -0.44, -17.41, 5.78, 1.93, 0.71, 0.02, 1.68, 1.59, 1.66, 1.59},
    {0.92, 0.36, 0.07, -1.49, -0.01, -3.60, 2.12, 0.83, 0.16, 1.67, 1.59, 1.66, 1.52},
    {0.89, 0.29, -0.01, -1.23, -19.08, 1.85, 2.07, 0.69, -0.02, 1.63, 1.59, 1.69, 1.53},
    {0.94, 0.31, 0.03, -2.50, 1.84, 0.04, 2.17, 0.72, 0.07, 1.63, 1.59, 1.67, 1.59},
    {0.90, 0.31, 0.02, -5.57, -16.21, 0.50, 2.06, 0.74, 0.05, 1.68, 1.54, 1.69, 1.60},
    {0.95, 0.32, 0.06, 6.23, -3.45, -3.68, 2.18, 0.72, 0.13, 1.65, 1.60, 1.68, 1.59},
    {0.91, 0.35, 0.10, -5.42, -0.04, -4.63, 2.09, 0.83, 0.23, 1.68, 1.58, 1.70, 1.60}
},
{
    {0.26, -0.26, 2.00, -35.57, 58.05, 16.32, 0.60, -0.64, 4.59, 1.96, 2.28, 2.20, 2.02},
    {0.13, -0.22, 2.00, -39.79, 178.99, 20.81, 0.24, -0.67, 4.59, 1.96, 2.20, 2.29, 2.01},
    {0.33, -0.08, 0.23, -36.25, 247.03, 16.69, 0.84, -0.11, 0.29, 2.03, 2.19, 2.23, 2.11},
    {0.65, 0.49, -1.37, -19.15, 146.75, 53.60, 1.59, 1.14, -3.16, 2.03, 2.25, 2.17, 2.11},
    {0.99, 0.66, -0.94, -28.89, 19.95, 11.81, 2.17, 1.53, -1.90, 1.94, 2.26, 2.21, 2.16},
    {0.39, 0.10, 0.78, -86.46, 69.46, -39.59, 0.86, 0.19, 1.87, 1.84, 2.15, 2.31, 2.03},
    {1.22, 0.15, 0.54, -286.72, 143.24, 44.01, 2.90, 0.74, 1.10, 1.74, 1.99, 2.08, 2.00},
    {1.00, 0.33, -0.08, -28.54, 46.85, -39.63, 2.39, 0.67, 0.01, 1.71, 1.69, 1.81, 1.79},
    {1.17, 0.22, -0.44, 82.97, 50.21, 38.82, 2.66, 0.47, -0.98, 1.73, 1.66, 1.77, 1.65},
    {0.87, 0.21, -0.28, 35.50, -6.66, 15.76, 2.10, 0.52, -0.62, 1.71, 1.61, 1.67, 1.68},
    {0.91, 0.42, -0.02, -29.30, 65.52, 20.64, 2.14, 0.90, -0.06, 1.67, 1.55, 1.62, 1.56},
    {1.00, 0.31, 0.01, -6.54, -12.79, 1.03, 2.11, 0.56, 0.01, 1.65, 1.59, 1.66, 1.59},
    {0.97, 0.33, -0.09, 12.82, -8.85, -3.66, 2.25, 0.74, -0.22, 1.64, 1.55, 1.63, 1.59},
    {0.89, 0.34, -0.03, 11.53, 3.68, -8.09, 2.08, 0.79, -0.11, 1.66, 1.55, 1.66, 1.51},
    {0.92, 0.25, -0.02, 10.66, -22.68, 7.07, 2.08, 0.60, -0.03, 1.65, 1.55, 1.66, 1.58},
    {0.97, 0.39, -0.05, -6.72, 24.19, -9.25, 2.25, 0.88, -0.09, 1.67, 1.56, 1.67, 1.58},
    {0.85, 0.31, 0.00, -1.79, -17.07, -0.50, 1.93, 0.74, 0.01, 1.62, 1.58, 1.62, 1.56},
    {0.98, 0.31, -0.03, 3.47, -1.37, 1.53, 2.26, 0.70, -0.07, 1.66, 1.58, 1.67, 1.58},
    {0.89, 0.32, -0.00, -5.00, -2.67, 1.96, 2.08, 0.75, -0.01, 1.66, 1.61, 1.64, 1.59},
    {0.94, 0.36, -0.01, -0.06, -3.75, 0.88, 2.15, 0.81, -0.04, 1.66, 1.57, 1.68, 1.59}
},
{
    {1000,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13}
},
{
    {100,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13}
},
{
    {16,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13}
},
{
    {17,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13}
},
{
    {15,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13}
},
{
    {14,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13}
},
{
    {500,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13}
},
{
    {0,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13},
    {1,2,3,4,5,6,7,8,9,10,11,12,13}
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
        int idx = 10;
        for(idx; idx < MAX_SIGNAL_LENGTH; idx++){
            if(score > scoreMatrix[idx][counter-1]){
                score = scoreMatrix[idx][counter-1];
            }
        }
		printf("DTW for library %d: Score = %f, idx = %d\n", i, score, idx);
		if(score < thresholds[i])
		{
            char str[64];
            sprintf(str, "Score: %4.2f", score);
            display_write(str, DISPLAY_LINE_1);
			return gestures[i];
		}
	}
	return 'N'; //no gesture satisfy
}

