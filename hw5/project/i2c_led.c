#include <stdio.h>
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

// device has default bus address of 0x76
#define ADDR _u(0b0100000)

#define LED_PIN 25

// hardware registers
#define REG_IODIR _u(0x00)
#define REG_GPIO _u(0x09)
#define REG_OLAT _u(0x0A)

void chip_init() {
    uint8_t buff[2];
    buff[0] = REG_IODIR;
    buff[1] = 0b01111111; // make GP7 output and all others input
    i2c_write_blocking(i2c_default, ADDR, buff, 2, false);
}

void set(char v) {
    uint8_t buff[2];
    buff[0] = REG_OLAT;
    buff[1] = v << 7;
    i2c_write_blocking(i2c_default, ADDR, buff, 2, false);
}

int read() {
    uint8_t buff[1];
    uint8_t reg = REG_GPIO;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buff, 1, false);  // false - finished with bus

    if (buff[0]&0b1 == 0b1) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    stdio_init_all();
    i2c_init(i2c_default, 100000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    chip_init();

    while (1) {
        if (read() == 1) {
            set(0);
        } else {
            set(1);
        }
        gpio_put(LED_PIN, 1);
        sleep_ms(200);
        gpio_put(LED_PIN, 0);
        sleep_ms(200);
    }
}