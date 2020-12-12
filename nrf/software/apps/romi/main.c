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
#include "controller.h"
#include "lsm9ds1.h"
#include "kobukiUtilities.h"

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_spi.h"

#include "buckler.h"
#include "display.h"
#include "kobukiActuator.h"
#include "kobukiSensorPoll.h"
#include "kobukiSensorTypes.h"
#include "kobukiUtilities.h"
#include "lsm9ds1.h"
#include "grip.h"
// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:49:xx:xx
        .platform_id       = 0x49,    // used as 4th octect in device BLE address
        .device_id         = 0x0000, // TODO: replace with your lab bench number
        .adv_name          = "Let's Go", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

// 32e61089-2b22-4db5-a914-43ce41986c70
static simple_ble_service_t letsgo_service = {{
    .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
                0xB5,0x4D,0x22,0x2B,0x89,0x10,0xE6,0x32}
}};

static simple_ble_char_t letsgo_IMU_char = {.uuid16 = 0x108a};
static char command; 
/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
    if (simple_ble_is_char_event(p_ble_evt, &letsgo_IMU_char)) {
      if(command != 'H' && command != 'L' && command != 'F' && command != 'B' && command != 'R' && command != 'U' && command != 'G')
      {
        printf("Unknown command: %x\n", command);
      }
      else 
      {
        cmd = command;       
      }

      //display_write(text_state, DISPLAY_LINE_0);
      // printf("Got write to LED characteristic!\n");
      // if (led_state) {
      //   printf("Turning on LED!\n");
      //   nrf_gpio_pin_clear(BUCKLER_LED0);
      // } else {
      //   printf("Turning off LED!\n");
      //   nrf_gpio_pin_set(BUCKLER_LED0);
      // }
    }
}
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);
int main(void) {

  // Initialize
  // initialize display
  nrf_drv_spi_t spi_instance = NRF_DRV_SPI_INSTANCE(1);
  nrf_drv_spi_config_t spi_config = {
    .sck_pin = BUCKLER_LCD_SCLK,
    .mosi_pin = BUCKLER_LCD_MOSI,
    .miso_pin = BUCKLER_LCD_MISO,
    .ss_pin = BUCKLER_LCD_CS,
    .irq_priority = NRFX_SPI_DEFAULT_CONFIG_IRQ_PRIORITY,
    .orc = 0,
    .frequency = NRF_DRV_SPI_FREQ_4M,
    .mode = NRF_DRV_SPI_MODE_2,
    .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
  };

  ret_code_t error_code = nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL);
  //printf("Check! %d\n", error_code);
  APP_ERROR_CHECK(error_code);
  display_init(&spi_instance);
  display_write("Hello, Human!", DISPLAY_LINE_0);
  printf("Display initialized!\n");

  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);
  lsm9ds1_init(&twi_mngr_instance);
  printf("IMU initialized!\n");
   // Setup PWM
  pwm_init();
  // grip(true);
  printf("PWM Init\n");

  // Setup LED GPIO
  nrf_gpio_cfg_output(BUCKLER_LED0);

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&letsgo_service);

 
  /*
  initialization of IMU & flexsensors
  initialization of the communication to the IMU & flexsors.
  */

  simple_ble_add_characteristic(1, 1, 1, 0,
       sizeof(char), (uint8_t*)&command,
       &letsgo_service, &letsgo_IMU_char);

  // Start Advertising
  simple_ble_adv_only_name();
  

  kobukiInit();
  printf("Kobuki initialized!\n");
  robot_state_t state = OFF;
  int counter = 0;
  while(1) {
    nrf_delay_ms(1);
    state = controller(state);
  }
}

