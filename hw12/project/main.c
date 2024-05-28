// based on adafruit and sparkfun libraries

#include <stdio.h>
#include <string.h>
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWMPin 22
char message[100];

int set_angle(int angle) { // function to set the angle of the servo
    float m = ((12.3 - 2.5) / 180.0);
    float b = 2.5;
    int dutyCycle = 100/(m * angle + b);
    int16_t wrap = 62500;
    return dutyCycle;
}

static inline void gpio_togg(uint gpio) { // function to toggle LED
    uint32_t mask = 1ul << gpio;
    if (gpio_get(gpio))
        gpio_clr_mask(mask);
    else
        gpio_set_mask(mask);
}

int main()
{
    stdio_init_all();
    printf("Start!\n");
    
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_set_function(PWMPin, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
    uint slice_num = pwm_gpio_to_slice_num(PWMPin); // Get PWM slice number
    float div = 40; // must be between 1-255
    pwm_set_clkdiv(slice_num, div); // divider
    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM
    
    while(1){
        gpio_togg(LED_PIN);
        sleep_ms(250);

        int dutyCycle = set_angle(0);
        pwm_set_gpio_level(PWMPin, wrap/dutyCycle);
        sleep_ms(2000);

        dutyCycle = set_angle(180);
        pwm_set_gpio_level(PWMPin, wrap/dutyCycle);
        sleep_ms(1500);
        
        gpio_togg(LED_PIN);
        sleep_ms(250);
    }
}