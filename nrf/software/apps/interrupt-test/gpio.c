#include "gpio.h"

// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
GPIO_REG* gpio = (GPIO_REG*)0x50000000;

void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
    printf("%p\n", &(gpio->PIN_CNF[0]));
    if(dir == OUTPUT)
    {
        ((gpio->DIR)) |= (1 << gpio_num);
    }
    else
    {
        ((gpio->DIR)) |= (0 << gpio_num);
        gpio->PIN_CNF[gpio_num] ^= (1 << 1);
    }
}

// Set gpio_num high
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num) {
    ((gpio->OUT)) |= (1 << gpio_num);
}

// Set gpio_num low
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num) {
    ((gpio->OUT)) ^= (1 << gpio_num);
}

// Inputs: 
//  gpio_num - gpio number 0-31
bool gpio_read(uint8_t gpio_num) {
    // should return pin state
    return (gpio->IN & (1 << gpio_num));
}
