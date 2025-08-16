#ifndef UART0_H
#define UART0_H

#include "LPC17xx.h"
#include <stdint.h>

// Power control for UART0
#define UART0_PWR       (1 << 3)      // PCUART0 bit

// Clock source select
#define RC_OSC_CLK      (1 << 0)      // Internal RC oscillator (4 MHz)

// PCLK divider mask for UART0 in PCLKSEL0
#define CCLKDIV         (3 << 6)      // Bits 6-7 mask

// Line control register bits
#define DATA_LEN        (3 << 0)      // 8-bit length
#define STOP_BIT        (1 << 2)      // Stop bit select (0 = 1 stop bit)
#define PARITY_EN       (1 << 3)      // Enable parity
#define BREAK_TR        (1 << 6)      // Break control

// Divisor latch access bit
#define D_LATCH         (1 << 7)

// Fractional divider register values
#define F_DIVADDVAL     (1 << 0)      // DIVADDVAL = 1
#define F_MULVAL        (12 << 4)     // MULVAL = 12

void uart0_init(void);
void uart0_byte_transmit(uint8_t data);
void uart0_str_transmit(uint8_t *str);
void uart0_byte_receive(uint8_t *byte);
void uart0_str_receive(uint8_t *str);
void delay_ms(uint32_t millis);
void new_line(void);

#endif
