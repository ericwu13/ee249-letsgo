/**

 */

#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"



#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/* TWI instance ID. */

#define TWI_INSTANCE_ID     0 // create a ID constant


// create a handle which will point to TWI instance, in this case its TWI_0
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

// a function to initialize the twi(i2c)
void twi_init(void)
{
  ret_code_t err_code; // a variable to hold error code

// Create a struct with configurations and pass the values to these configurations.
  const nrf_drv_twi_config_t twi_config = {
    .scl                = 27, // scl connected to pin 22, you can change it to any other pin
    .sda                = 26, // sda connected to pin 23, you can change it to any other pin
    .frequency          = NRF_DRV_TWI_FREQ_100K, // set the communication speed to 100K, we can select 250k or 400k as well
    .interrupt_priority = APP_IRQ_PRIORITY_HIGH, // Interrupt priority is set to high, keep in mind to change it if you are using a soft-device
    .clear_bus_init     = false // automatic bus clearing 

  };

  err_code = nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL); // initialize the twi
  APP_ERROR_CHECK(err_code); // check if any error occured during initialization

  nrf_drv_twi_enable(&m_twi); // enable the twi comm so that its ready to communicate with the sensor

}

/**
 * @brief Function for main application entry.
 */
int main(void)
{

  ret_code_t err_code; // a variable to hold error code value
  uint8_t address = 0x6B; // address of the sensor
  uint8_t sample_data = 0x00; // sample data initialized with 0 value.

// initialize the Logger so that we can print msgs on the logger
  APP_ERROR_CHECK(NRF_LOG_INIT(NULL)); 
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  NRF_LOG_INFO("Application Started");

  NRF_LOG_FLUSH(); // flushing is necessary if deferred is set to 1(check this video tutorial to know it better)

  twi_init(); // call the twi initialization function

// read some data from the sensor
  err_code = nrf_drv_twi_rx(&m_twi, address, &sample_data, sizeof(sample_data));
  if(err_code == NRF_SUCCESS) // if reading data was successful
  {

    NRF_LOG_INFO("Successfully detected a device at address: 0x%x", address); // let the users know its working
  
    }

    NRF_LOG_FLUSH(); // flushing is important, if you set the deffered to 1, if deffered is set to 0 then we don't need to flush the log buffer.
  


    while (true)
    {
        /* Empty loop. */
    }
}

/** @} */
