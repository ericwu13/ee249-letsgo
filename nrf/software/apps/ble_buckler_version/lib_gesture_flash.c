#include "lib_gesture_flash.h"

const float thresholds[LIBRARY_SIZE] = {100,100,100,100,100,100,
										100,100,100,100,100,100,
										100,100,100,100,100,100,
										100,100,100,100,100,100};

const char  gestures  [LIBRARY_SIZE] = "FFFFLLLLRRRRBBBBSSSSGGGG";

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

const float training_data[LIBRARY_SIZE][MAX_SIGNAL_LENGTH][NUM_IMU_DATA] = {{
    {142,2,3,4,5,6,7,8,9,10,11,12,13},
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
    {1757,2,3,4,5,6,7,8,9,10,11,12,13},
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
    {175,2,3,4,5,6,7,8,9,10,11,12,13},
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
    {141,2,3,4,5,6,7,8,9,10,11,12,13},
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
},
{
    {142,2,3,4,5,6,7,8,9,10,11,12,13},
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
    {1757,2,3,4,5,6,7,8,9,10,11,12,13},
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
    {175,2,3,4,5,6,7,8,9,10,11,12,13},
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
    {141,2,3,4,5,6,7,8,9,10,11,12,13},
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

char dtw(float (*scoreMatrix)[MAX_SIGNAL_LENGTH], float (*signal)[NUM_IMU_DATA], int counter){
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
		float score = scoreMatrix[MAX_SIGNAL_LENGTH-1][counter-1];
		printf("Counter = %d, Score = %f\n", counter, score);
		if(score < thresholds[i])
		{
			return gestures[i];
		}
	}
	return 'N'; //no gesture satisfy
}

