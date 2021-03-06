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
#include "app_error.h"
#include "nrf_sdh.h"
#include "nrfx_gpiote.h"
#include "nrf_serial.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_timer.h"


#include "buckler.h"
#include "display.h"
#include "kobukiActuator.h"
#include "kobukiSensorPoll.h"
#include "kobukiSensorTypes.h"
#include "kobukiUtilities.h"
#include "lsm9ds1.h"

#include "lib_gesture_flash.h"
#define EXTERNAL_IMU

/* Global Variables */
typedef float imu_data_type ;//float

//float IMU_data[NUM_IMU_DATA];
volatile bool moved = false;
volatile int counter = 0;
float (*signal_ptr)[NUM_IMU_DATA];
char gesture_ble = 'N';

NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);
const nrf_drv_timer_t timeout_timer = NRF_DRV_TIMER_INSTANCE(3);

void read_IMU(imu_data_type* data, int length)
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

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:49:xx:xx
        .platform_id       = 0x49,    // used as 4th octect in device BLE address
        .device_id         = 0x0000, // TODO: replace with your lab bench number
        .adv_name          = "Let's Go", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(7.5, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(7.5, UNIT_1_25_MS),
};

// 32e61089-2b22-4db5-a914-43ce41986c70
static simple_ble_service_t letsgo_service = {{
    .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
                0xB5,0x4D,0x22,0x2B,0x89,0x10,0xE6,0x32}
}};


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
            if(counter < MAX_SIGNAL_LENGTH) {
                read_IMU(signal_ptr[counter], NUM_IMU_DATA);
                counter ++;
                // getAccelIntSrc();
                printf("get data %d\n", counter);
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

void GPIOTE_IRQHandler(void) {
    //NRF_GPIOTE->INTENCLR |= (uint32_t) 1;
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    if(!moved && counter == 0) {
        printf("Motion Detected\n");
        moved = true;
        //counter = 0;
        uint32_t time_ticks = nrf_drv_timer_ms_to_ticks(&timeout_timer, 50);
        nrf_drv_timer_extended_compare(
            &timeout_timer, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
        nrf_drv_timer_enable(&timeout_timer);

    }
}

static simple_ble_char_t letsgo_accel_char = {.uuid16 = 0x108a};
// static simple_ble_char_t letsgo_gyro_char = {.uuid16 = 0x108b};
// static simple_ble_char_t letsgo_magnet_char = {.uuid16 = 0x108c};
// static simple_ble_char_t letsgo_flex_char = {.uuid16 = 0x108d};

//static imu_data_type IMU_data[NUM_IMU_DATA];

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
    if (simple_ble_is_char_event(p_ble_evt, &letsgo_accel_char)) {
      printf("Got write to Accel!\n");
    }
    // if (simple_ble_is_char_event(p_ble_evt, &letsgo_gyro_char)) {
    //   printf("Got write to Gyro!\n");
    // }
    // if (simple_ble_is_char_event(p_ble_evt, &letsgo_magnet_char)) {
    //   printf("Got write to Magnet!\n");
    // }
    // if (simple_ble_is_char_event(p_ble_evt, &letsgo_flex_char)) {
    //   printf("Got write to Flex!\n");
    // }
}

void print_IMU(imu_data_type* data, int length)
{
    printf("Accel: (%4.2f, %4.2f, %4.2f)\n", data[0], data[1], data[2]);
    printf("Gyro: (%4.2f, %4.2f, %4.2f)\n", data[3], data[4], data[5]);
    printf("Maget: (%4.2f, %4.2f, %4.2f)\n", data[6], data[7], data[8]);
    printf("Flex: (%4.2f, %4.2f, %4.2f, %4.2f)\n\n", data[9], data[10], data[11], data[12]);
}

int main(void) {
    // initialize IMU
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
    // initilize flex
    saadc_init();
    printf("ADC Interface Init\n");

    // initialize timer
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    nrf_drv_timer_init(&timeout_timer, &timer_cfg, timeout_IRQ);
    printf("Timeout Timer Init\n");

    // initialize GPIO interrupt
    #ifdef EXTERNAL_IMU
    interrupt_init(14);
    nrf_gpio_cfg_input(14, NRF_GPIO_PIN_PULLUP);
    #else
    interrupt_init(28);
    nrf_gpio_cfg_input(28, NRF_GPIO_PIN_PULLUP);
    #endif
    printf("GPIO Interrupt Init\n");

    

    NVIC_SetPriority (GPIOTE_IRQn, 2);
    NVIC_SetPriority (TIMER3_IRQn, 1);
    NVIC_SetPriority (SPI0_TWI0_IRQn, 0);


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
    APP_ERROR_CHECK(error_code);
    display_init(&spi_instance);
    display_write("Hello, Human!", DISPLAY_LINE_0);
    printf("Display Init\n");

    // Setup LED GPIO
    nrf_gpio_cfg_output(BUCKLER_LED0);

    //Setup BLE
    simple_ble_app = simple_ble_init(&ble_config);
 
    simple_ble_add_service(&letsgo_service);

    simple_ble_add_characteristic(1, 1, 1, 0,
       sizeof(char), (uint8_t*)&(gesture_ble),
       &letsgo_service, &letsgo_accel_char
    );
    simple_ble_adv_only_name();


    float scoreMatrix[MAX_SIGNAL_LENGTH][MAX_SIGNAL_LENGTH];
    float signal[MAX_SIGNAL_LENGTH][NUM_IMU_DATA];
    signal_ptr = signal;
    int timers = 0;
    char gesture_dtw_result = 'N';
    while(1) {

        printf("Waiting for movement.....\n");
        // idle and collect data
        while(gesture_dtw_result == 'N'){
            while(counter != MAX_SIGNAL_LENGTH){ getAccelIntSrc(); nrf_delay_ms(30); display_write("Waiting..", DISPLAY_LINE_0);}// wait for full data;
            //while(gesture_dtw_result == 'N'){
            // dtw
            int dtw_counter = counter;
            gesture_dtw_result = dtw(scoreMatrix, signal, dtw_counter);
            if(gesture_dtw_result == 'N'){
                display_write("No Action!", DISPLAY_LINE_0);
            }      
            if(dtw_counter == MAX_SIGNAL_LENGTH){
                counter = 0;
            }
        }
        printf("Gesture detected! Result: %c\n", gesture_dtw_result);
        switch(gesture_dtw_result){
            case 'S':
                display_write("Stop", DISPLAY_LINE_0); break;
            case 'F':
                display_write("Forward", DISPLAY_LINE_0); break;
            case 'B':
                display_write("Backward", DISPLAY_LINE_0); break;
            case 'R':
                display_write("Right", DISPLAY_LINE_0); break;
            case 'L':
                display_write("Left", DISPLAY_LINE_0); break;
            case 'G':
                display_write("Grip", DISPLAY_LINE_0); break; 
            default:
                display_write("Error", DISPLAY_LINE_0); break;                
        }
        

        gesture_ble = gesture_dtw_result;
        gesture_dtw_result = 'N';

        error_code = simple_ble_notify_char(&letsgo_accel_char);
        APP_ERROR_CHECK(error_code);
        nrf_delay_ms(1000);
    }
}
