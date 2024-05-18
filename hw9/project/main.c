#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARTIY  UART_PARITY_NONE

#define UART_RX_PIN 1
#define UART_TX_PIN 0

volatile int i = 0;
char m[100];

// RX interrupt handler

void on_uart_rx();


int main() {
    stdio_init_all();

    // UART SETUP
    uart_init(UART_ID, 2400);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow controls CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARTIY);

    // Turn off FIFO's - we want to do this char by char
    uart_set_fifo_enabled(UART_ID, false);

    // Set up RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("Start!\n");

    while (1) {
        int ifromcomp = 0;
        scanf("%d", &ifromcomp);
        printf("Comp sent: %d\n", ifromcomp);

        char txm[100];
        sprintf(txm, "%d\n", ifromcomp);
        uart_puts(UART_ID, txm);

        sleep_ms(250);
    }
}

void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        if (ch == '\n') {
            m[i] = 0;
            i = 0;
            printf("From Zero: %s\n", m);
        } else {
            m[i] = ch;
            i++;
        }
    }
}