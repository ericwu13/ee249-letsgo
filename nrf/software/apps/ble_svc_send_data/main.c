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

#define NUM_IMU_DATA 13
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

static simple_ble_char_t letsgo_accel_char = {.uuid16 = 0x108a};
static simple_ble_char_t letsgo_gyro_char = {.uuid16 = 0x108b};
static simple_ble_char_t letsgo_magnet_char = {.uuid16 = 0x108c};
static simple_ble_char_t letsgo_flex_char = {.uuid16 = 0x108d};
static float IMU_data[NUM_IMU_DATA];

void saadc_callback_handler(nrf_drv_saadc_evt_t const * p_event){}
void saadc_init(void)
{
  ret_code_t err_code;

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
  err_code = nrf_drv_saadc_init(NULL, saadc_callback_handler);
  APP_ERROR_CHECK(err_code);
  err_code = nrfx_saadc_channel_init(0, &channel_config0);
  APP_ERROR_CHECK(err_code);
  err_code = nrfx_saadc_channel_init(1, &channel_config1);
  APP_ERROR_CHECK(err_code);
  err_code = nrfx_saadc_channel_init(2, &channel_config2);
  APP_ERROR_CHECK(err_code);
  err_code = nrfx_saadc_channel_init(3, &channel_config3);
  APP_ERROR_CHECK(err_code);
}
void log_init(void)
{
  APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
  NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
    if (simple_ble_is_char_event(p_ble_evt, &letsgo_accel_char)) {
      printf("Got write to Accel!\n");
    }
    if (simple_ble_is_char_event(p_ble_evt, &letsgo_gyro_char)) {
      printf("Got write to Gyro!\n");
    }
    if (simple_ble_is_char_event(p_ble_evt, &letsgo_magnet_char)) {
      printf("Got write to Magnet!\n");
    }
    if (simple_ble_is_char_event(p_ble_evt, &letsgo_flex_char)) {
      printf("Got write to Flex!\n");
    }
}

void read_IMU(float* data, int length)
{
    lsm9ds1_measurement_t accel_val = lsm9ds1_read_accelerometer();
    lsm9ds1_measurement_t gyro_val = lsm9ds1_read_gyro();
    lsm9ds1_measurement_t magnet_val = lsm9ds1_read_magnetometer();
    nrf_saadc_value_t adc_val0;
    nrf_saadc_value_t adc_val1;
    nrf_saadc_value_t adc_val2;
    nrf_saadc_value_t adc_val3;
    nrfx_saadc_sample_convert(0, &adc_val0);
    nrfx_saadc_sample_convert(1, &adc_val1);
    nrfx_saadc_sample_convert(2, &adc_val2);
    nrfx_saadc_sample_convert(3, &adc_val3);
    data[0] = accel_val.x_axis;
    data[1] = accel_val.y_axis;
    data[2] = accel_val.z_axis;
    data[3] = gyro_val.x_axis;
    data[4] = gyro_val.y_axis;
    data[5] = gyro_val.z_axis;
    data[6] = magnet_val.x_axis;
    data[7] = magnet_val.y_axis;
    data[8] = magnet_val.z_axis;
    data[9] =  adc_val0 * 1.8 / 512;
    data[10] = adc_val1 * 1.8 / 512;
    data[11] = adc_val2 * 1.8 / 512;
    data[12] = adc_val3 * 1.8 / 512;
    return;
}
void print_IMU(float* data, int length)
{
  printf("Accel: (%4.2f, %4.2f, %4.2f)\n", data[0], data[1], data[2]);
  printf("Gyro: (%4.2f, %4.2f, %4.2f)\n", data[3], data[4], data[5]);
  printf("Maget: (%4.2f, %4.2f, %4.2f)\n", data[6], data[7], data[8]);
  printf("Flex: (%4.2f, %4.2f, %4.2f, %4.2f)\n\n", data[9], data[10], data[11], data[12]);
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

  error_code = nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL);
  //printf("Check! %d\n", error_code);
  APP_ERROR_CHECK(error_code);
  display_init(&spi_instance);
  display_write("Hello, Human!", DISPLAY_LINE_0);
  printf("Display initialized!\n");

  // Setup LED GPIO
  nrf_gpio_cfg_output(BUCKLER_LED0);

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&letsgo_service);

  /*
  initialization of IMU & flexsensors
  initialization of the communication to the IMU & flexsors.
  */
  log_init();
  saadc_init();
  NRF_LOG_INFO("Application Started!!!");

  simple_ble_add_characteristic(1, 1, 1, 0,
       sizeof(float) * 3, (uint8_t*)&(IMU_data[0]),
       &letsgo_service, &letsgo_accel_char);

  simple_ble_add_characteristic(1, 1, 1, 0,
       sizeof(float) * 3, (uint8_t*)&(IMU_data[3]),
       &letsgo_service, &letsgo_gyro_char);

  simple_ble_add_characteristic(1, 1, 1, 0,
       sizeof(float) * 3, (uint8_t*)&(IMU_data[6]),
       &letsgo_service, &letsgo_magnet_char);
  simple_ble_add_characteristic(1, 1, 1, 0,
       sizeof(float) * 4, (uint8_t*)&(IMU_data[9]),
       &letsgo_service, &letsgo_flex_char);
  // Start Advertising
  simple_ble_adv_only_name();

  while(1) {
    nrf_delay_ms(1000);
    read_IMU(IMU_data, NUM_IMU_DATA);
    error_code = simple_ble_notify_char(&letsgo_accel_char);
    error_code = simple_ble_notify_char(&letsgo_gyro_char);
    error_code = simple_ble_notify_char(&letsgo_magnet_char);
    error_code = simple_ble_notify_char(&letsgo_flex_char);
    APP_ERROR_CHECK(error_code);
    print_IMU(IMU_data, NUM_IMU_DATA);
  }
}

