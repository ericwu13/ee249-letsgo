// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_gpio.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "software_interrupt.h"

#include "buckler.h"
#include "gpio.h"

void SWI1_EGU1_IRQHandler(void) {
    NRF_EGU1->EVENTS_TRIGGERED[0] = 0;
    printf("Start of Software Interrupt\n");
    nrf_delay_ms(5000);
    printf("End of Software Interrupt\n");
}

void GPIOTE_IRQHandler(void) {
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    printf("Hardware Interrupt\n");
    gpio_clear(25);
    nrf_delay_ms(100);
    gpio_set(25);
    // NRF_GPIOTE->EVENTS_IN[0] = (uint32_t*) GPIOTE_IRQHandler; // Qusetion 2: why we don't have to set events_in back to handler
}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  // error_code = NRF_LOG_INIT(NULL);
  // APP_ERROR_CHECK(error_code);
  // NRF_LOG_DEFAULT_BACKENDS_INIT();
  // printf("Log initialized!\n");
  
  // set Event Mode
  NRF_GPIOTE->CONFIG[0] |= (uint32_t) 1;

  // set Button0
  NRF_GPIOTE->CONFIG[0] &= ~((uint32_t) 31 << 8);
  NRF_GPIOTE->CONFIG[0] |= ((uint32_t) 28 << 8);

  // set HitoLo
  NRF_GPIOTE->CONFIG[0] &= ~((uint32_t) 2 << 16);
  NRF_GPIOTE->CONFIG[0] |= ((uint32_t) 2 << 16);

  // enable IN[0]
  NRF_GPIOTE->INTENSET |= (uint32_t) 1;
  // NRF_GPIOTE->EVENTS_IN[0] = (uint32_t*) GPIOTE_IRQHandler; // Question 1": why we have to set it
  NVIC_EnableIRQ(GPIOTE_IRQn);
  
  // software interrupt
  software_interrupt_init();
  
  NRF_EGU1->EVENTS_TRIGGERED[0] = (uint32_t*) SWI1_EGU1_IRQHandler;

  NVIC_SetPriority ( SWI1_EGU1_IRQn , 0);
  NVIC_SetPriority ( GPIOTE_IRQn , 0);

  gpio_config(25, OUTPUT);
  gpio_config(22, INPUT);
  gpio_set(25);

  // loop forever
  while (1) {
    software_interrupt_generate();
    /*if(gpio_read(22))
      __WFI ();*/
    printf("Looping\n");
    nrf_delay_ms(1000);
  }
}

