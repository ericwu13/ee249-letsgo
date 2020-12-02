// BLE Service Template
//
// Creates a service for changing LED state over BLE

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "math.h"
#include "app_util.h"
#include "nrf_twi_mngr.h"
#include "nrf_gpio.h"
#include "display.h"
#include "nrf_delay.h"

#include "simple_ble.h"
#include "buckler.h"
#include "lsm9ds1.h"
#include "max44009.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_spi.h"

#include "kobukiActuator.h"
#include "kobukiSensorPoll.h"
#include "kobukiSensorTypes.h"
#include "kobukiUtilities.h"

#define NUM_IMU_DATA 14
static float IMU_data[NUM_IMU_DATA];
static bool volatile moved = false;

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state


void read_IMU(float* data, int length)
{
    lsm9ds1_measurement_t accel_val = lsm9ds1_read_accelerometer();
    lsm9ds1_measurement_t gyro_val = lsm9ds1_read_gyro();
    lsm9ds1_measurement_t magnet_val = lsm9ds1_read_magnetometer();
    data[0] = accel_val.x_axis;
    data[1] = accel_val.y_axis;
    data[2] = accel_val.z_axis;
    data[3] = gyro_val.x_axis;
    data[4] = gyro_val.y_axis;
    data[5] = gyro_val.z_axis;
    data[6] = magnet_val.x_axis;
    data[7] = magnet_val.y_axis;
    data[8] = magnet_val.z_axis;
    // TODO: read flex sensor
    return;
}
void print_IMU(float* data, int length)
{
  printf("Accel: (%4.2f, %4.2f, %4.2f)\n", data[0], data[1], data[2]);
  // printf("Gyro: (%4.2f, %4.2f, %4.2f)\n", data[3], data[4], data[5]);
  // printf("Maget: (%4.2f, %4.2f, %4.2f)\n", data[6], data[7], data[8]);
  // printf("Flex: (%4.2f, %4.2f, %4.2f, %4.2f, %4.2f)\n\n", data[9], data[10], data[11], data[12], data[13]);
}

void GPIOTE_IRQHandler(void) {
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    // printf("Motion Detected\n");
    //read_IMU(IMU_data, NUM_IMU_DATA);
    //printf("Accel: (%4.2f, %4.2f, %4.2f)\n", IMU_data[0], IMU_data[1], IMU_data[2]);
    moved = true;
    lsm9ds1_start_gyro_integration();
    // NRF_GPIOTE->EVENTS_IN[0] = (uint32_t*) GPIOTE_IRQHandler; // Qusetion 2: why we don't have to set events_in back to handler
}

bool isStop(float* data) {
    if(fabs(data[0]) < 16000 &&
       fabs(data[1]) < 16000 &&
       fabs(data[2]) < 16000) return true;;
    return false;
}


NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);
int main(void) {

  // Initialize
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = 27;
  i2c_config.sda = 26;
  i2c_config.frequency = NRF_TWIM_FREQ_400K;
  ret_code_t error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);
  lsm9ds1_init(&twi_mngr_instance);
  printf("IMU initialized!\n");
  lsm9ds1_intcfg();
  printf("IMU Interrupt Init\n");


  // Setup LED GPIO
  nrf_gpio_cfg_output(BUCKLER_LED0);
 

  // set 14 to be interrupt
  NRF_GPIOTE->CONFIG[0] |= (uint32_t) 1;
  NRF_GPIOTE->CONFIG[0] &= ~((uint32_t) 31 << 8);
  NRF_GPIOTE->CONFIG[0] |= ((uint32_t) 14 << 8);

  // set HitoLo
  NRF_GPIOTE->CONFIG[0] &= ~((uint32_t) 3 << 16);
  NRF_GPIOTE->CONFIG[0] |= ((uint32_t) 2 << 16);

  // enable IN[0]
  NRF_GPIOTE->INTENSET |= (uint32_t) 1;
  // NRF_GPIOTE->EVENTS_IN[0] = (uint32_t*) GPIOTE_IRQHandler; // Question 1": why we have to set it
  NVIC_EnableIRQ(GPIOTE_IRQn);
  NVIC_SetPriority (GPIOTE_IRQn , 0);
  nrf_gpio_cfg_input(14, NRF_GPIO_PIN_PULLUP);


  /*
  initialization of IMU & flexsensors
  initialization of the communication to the IMU & flexsors.
  */

  int counter = 0;
  while(1) {
    nrf_delay_ms(1);
    getAccelIntSrc();
    //getGyroIntSrc();
    //printf("%ld\n", getAccelIntSrc());
    //print_IMU(IMU_data, NUM_IMU_DATA);
    if(moved == true) {
        read_IMU(IMU_data, NUM_IMU_DATA);
        printf("Speed: %4.2f, %4.2f, %4.2f\n", integrated_angle.x_axis, integrated_angle.y_axis, integrated_angle.z_axis);
        counter++;
        if(isStop(IMU_data)) {
            printf("Length of Data: %d\n", counter);

            moved = false;
            counter = 0;
            lsm9ds1_stop_gyro_integration();
        }
    }
    //printf("%ld\n", getGyroIntSrc());
    // printf("Interrupt: %ld\n", nrf_gpio_pin_read(14));
  }
}

