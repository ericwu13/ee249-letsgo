// BLE Service Template
//
// Creates a service for changing LED state over BLE

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
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

#include <stdio.h>
#include <string.h>
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "boards.h"
#include "app_error.h"
#include "app_util_platform.h"

/* Create an empty handler and pass this handler in the saadc initialization function
  > Normally this handler deals with the adc events but we are using blocking mode
  > In blocking mode the functions are called and the processor waits for the adc to finish taking samples from the respective channels
  > Event handler will not be called in this method
*/

void saadc_callback_handler(nrf_drv_saadc_evt_t const * p_event)
{
 // Empty handler function
}




// Create a function which configures the adc input pins and channels as well as the mode of operation of adc

void saadc_init(void)
{
	// A variable to hold the error code
  ret_code_t err_code;

  // Create a config struct and assign it default values along with the Pin number for ADC Input
  // Configure the input as Single Ended(One Pin Reading)
  // Make sure you allocate the right pin.
  nrf_saadc_channel_config_t channel_config0 = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);
  nrf_saadc_channel_config_t channel_config1 = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);
  nrf_saadc_channel_config_t channel_config2 = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);
  nrf_saadc_channel_config_t channel_config3 = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN3);
  /* 
  AIN0 P0.02
  AIN1 P0.03
  AIN2 P0.04
  AIN3 P0.05
  */
  // Initialize the saadc 
  err_code = nrf_drv_saadc_init(NULL, saadc_callback_handler);
  APP_ERROR_CHECK(err_code);

  // Initialize the Channel which will be connected to that specific pin.
  err_code = nrfx_saadc_channel_init(0, &channel_config0);
  APP_ERROR_CHECK(err_code);
  err_code = nrfx_saadc_channel_init(1, &channel_config1);
  APP_ERROR_CHECK(err_code);
  err_code = nrfx_saadc_channel_init(2, &channel_config2);
  APP_ERROR_CHECK(err_code);
  err_code = nrfx_saadc_channel_init(3, &channel_config3);
  APP_ERROR_CHECK(err_code);
}



// A function which will initialize the Log module for us
void log_init(void)
{
	// check if any error occurred during its initialization
  APP_ERROR_CHECK(NRF_LOG_INIT(NULL));

	// Initialize the log backends module
  NRF_LOG_DEFAULT_BACKENDS_INIT();

}

int main(void)
{
	
	// call the log initialization function
  log_init();

  // call the saadc initialization function created above
  saadc_init();

  // a struct to hold 16-bit value, create a variable of this type because our input resolution may vary from 8 bit to 14 bits depending on our configurations
  // this variable holds the adc sample value
  nrf_saadc_value_t adc_val0;
  nrf_saadc_value_t adc_val1;
  nrf_saadc_value_t adc_val2;
  nrf_saadc_value_t adc_val3;

  // Print a simple msg that everything started without any error
  NRF_LOG_INFO("Application Started!!!");
  // Inifinite loop
  while (1)
  {
      // a blocking function which will be called and the processor waits until the value is read
      // the sample value read is in 2's complement and is automatically converted once retrieved
      // first parameter is for the adc input channel 
      // second parameter is to pass the address of the variable in which we store our adc sample value
      nrfx_saadc_sample_convert(0, &adc_val0);
      nrfx_saadc_sample_convert(1, &adc_val1);
      nrfx_saadc_sample_convert(2, &adc_val2);
      nrfx_saadc_sample_convert(3, &adc_val3);

      // use nrf log and float marker to show the floating point values on the log
      //maybe times 3.6...
      NRF_LOG_INFO("Volts 0: " NRF_LOG_FLOAT_MARKER "\n", NRF_LOG_FLOAT(adc_val0 * 1.8 / 512));
      NRF_LOG_INFO("Volts 1: " NRF_LOG_FLOAT_MARKER "\n", NRF_LOG_FLOAT(adc_val1 * 1.8 / 512));
      NRF_LOG_INFO("Volts 2: " NRF_LOG_FLOAT_MARKER "\n", NRF_LOG_FLOAT(adc_val2 * 1.8 / 512));
      NRF_LOG_INFO("Volts 3: " NRF_LOG_FLOAT_MARKER "\n", NRF_LOG_FLOAT(adc_val3 * 1.8 / 512));
       
      // give 500ms delay 
      nrf_delay_ms(500);
    }
}


/** @} */
