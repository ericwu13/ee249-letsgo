// BLE Service Template
//
// Creates a service for changing LED state over BLE

#define EXTERNAL_IMU

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
#include "nrf_drv_timer.h"

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
static float IMU_data[NUM_IMU_DATA];
static bool volatile moved = false;
static int counter = 0;
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);
static const nrf_drv_timer_t timeout_timer = NRF_DRV_TIMER_INSTANCE(3);
static const nrf_drv_timer_t read_timer = NRF_DRV_TIMER_INSTANCE(4);

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Initialization

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
    // TODO: read flex sensor
    data[9] =  adc_val0 * 1.8 / 512;
    data[10] = adc_val1 * 1.8 / 512;
    data[11] = adc_val2 * 1.8 / 512;
    data[12] = adc_val3 * 1.8 / 512;
    return;
}

void print_IMU(float* data, int length)
{
    //printf("Accel: (%4.2f, %4.2f, %4.2f)\n", data[0], data[1], data[2]);
    for(int i = 0; i < 12; ++i) {
      printf("%4.2f ", data[i]);
    }
    printf("%4.2f\n", data[12]);
    // printf("Gyro: (%4.2f, %4.2f, %4.2f)\n", data[3], data[4], data[5]);
    // printf("Maget: (%4.2f, %4.2f, %4.2f)\n", data[6], data[7], data[8]);
    // printf("Flex: (%4.2f, %4.2f, %4.2f, %4.2f, %4.2f)\n\n", data[9], data[10], data[11], data[12], data[13]);
}

void log_init(void)
{
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}
void saadc_callback_handler(nrf_drv_saadc_evt_t const * p_event){}
void saadc_init(void) {
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
void timeout_IRQ(nrf_timer_event_t event_type, void* p_context) {
    switch (event_type) {
        case NRF_TIMER_EVENT_COMPARE0:
            if(counter < 20) {
                counter ++;
                read_IMU(IMU_data, NUM_IMU_DATA);
                print_IMU(IMU_data, 13);
                // getAccelIntSrc();
                // printf("get data\n");
                // lsm9ds1_read_accelerometer();
                // uint32_t time_ticks = nrf_drv_timer_ms_to_ticks(&timeout_timer, 1000);
                // nrf_drv_timer_clear(&timeout_timer);
                // nrf_drv_timer_compare(
                //     &timeout_timer, NRF_TIMER_CC_CHANNEL0, time_ticks, true);
                
            } else {
                moved = false;
                printf("time out!\n");
                printf("Length of Data: %d\n", counter);
                nrf_drv_timer_disable(&timeout_timer);
            }
            
            break;
        default:
            break;
    }
}


void interrupt_init(uint8_t pin) {
    // set 14 to be interrupt
    NRF_GPIOTE->CONFIG[0] |= (uint32_t) 1;
    NRF_GPIOTE->CONFIG[0] &= ~((uint32_t) 31 << 8);
    NRF_GPIOTE->CONFIG[0] |= ((uint32_t) pin << 8);
    // set HitoLo
    NRF_GPIOTE->CONFIG[0] &= ~((uint32_t) 3 << 16);
    NRF_GPIOTE->CONFIG[0] |= ((uint32_t) 2 << 16);

    // enable IN[0]
    NRF_GPIOTE->INTENSET |= (uint32_t) 1;
    // NRF_GPIOTE->EVENTS_IN[0] = (uint32_t*) GPIOTE_IRQHandler; // Question 1": why we have to set it
    NVIC_EnableIRQ(GPIOTE_IRQn);
}


// IRQ and Functions
void GPIOTE_IRQHandler(void) {
    //NRF_GPIOTE->INTENCLR |= (uint32_t) 1;
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    if(!moved) {
        printf("Motion Detected\n");
        moved = true;
        counter = 0;
        uint32_t time_ticks = nrf_drv_timer_ms_to_ticks(&timeout_timer, 50);
        nrf_drv_timer_extended_compare(
            &timeout_timer, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
        nrf_drv_timer_enable(&timeout_timer);

    }
}





int main(void) {

    /*
    initialization of IMU & flexsensors
    initialization of the communication to the IMU & flexsors.
    */
    log_init();

    nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
    #ifdef EXTERNAL_IMU
    i2c_config.scl = 27;
    i2c_config.sda = 26;
    #else
    i2c_config.scl = BUCKLER_SENSORS_SCL;
    i2c_config.sda = BUCKLER_SENSORS_SDA;
    #endif
    
    i2c_config.frequency = NRF_TWIM_FREQ_100K;
    ret_code_t error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
    APP_ERROR_CHECK(error_code);
    #ifdef EXTERNAL_IMU
    ext_lsm9ds1_init(&twi_mngr_instance);
    #else
    lsm9ds1_init(&twi_mngr_instance);
    #endif
    
    printf("IMU initialized!\n");
    lsm9ds1_intcfg();
    printf("IMU Interrupt Init\n");

    // Setup LED GPIO
    nrf_gpio_cfg_output(BUCKLER_LED0);

    // set 14 to be interrupt
    #ifdef EXTERNAL_IMU
    interrupt_init(14);
    nrf_gpio_cfg_input(14, NRF_GPIO_PIN_PULLUP);
    #else
    interrupt_init(28);
    nrf_gpio_cfg_input(28, NRF_GPIO_PIN_PULLUP);
    #endif
    
    NRF_LOG_INFO("Interrupt Init");
    saadc_init();
    NRF_LOG_INFO("ADC Interface Init");
    
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    nrf_drv_timer_init(&timeout_timer, &timer_cfg, timeout_IRQ);
    NRF_LOG_INFO("Timeout Timer Init");
    // nrf_drv_timer_init(&read_timer, &timer_cfg, read_IRQ);
    // uint32_t read_ticks = nrf_drv_timer_ms_to_ticks(&read_timer, 500);
    // nrf_drv_timer_extended_compare(&read_timer, NRF_TIMER_CC_CHANNEL0, read_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    // nrf_drv_timer_enable(&read_timer);
    NRF_LOG_INFO("Read Timer Init");

    NVIC_SetPriority (GPIOTE_IRQn, 2);
    NVIC_SetPriority (TIMER3_IRQn, 1);
    NVIC_SetPriority (SPI0_TWI0_IRQn, 0);

    NRF_LOG_INFO("Application Started!!!");
    //nrf_delay_ms(3000);
    //timer_start(3000000);
    nrf_delay_ms(500);
    printf("start recording\n");

    while(1) {
        getAccelIntSrc();
        nrf_delay_ms(10);
        // printf("123\n");
        // uint8_t a = getAccelIntSrc();
        // printf("%d\n", a);
        // read_IMU(IMU_data, NUM_IMU_DATA);
        // if(moved == true) {
        //     read_IMU(IMU_data, NUM_IMU_DATA);
        //     counter++;
        // print_IMU(IMU_data, 13);
        //     printf("Length of Data: %d\n", counter);
        // }
    }
}

