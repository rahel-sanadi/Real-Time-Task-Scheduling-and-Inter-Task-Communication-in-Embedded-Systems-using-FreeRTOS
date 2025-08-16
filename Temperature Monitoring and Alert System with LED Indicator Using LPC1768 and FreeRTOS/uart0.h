#ifndef UART0_H
#define UART0_H

#include "LPC17xx.h"
#include <stdint.h>

// UART0 power and clock
#define UART0_PWR       (1 << 3)
#define RC_OSC_CLK      (1 << 0)
#define CCLKDIV         (3 << 6)

// UART line control
#define DATA_LEN        (3 << 0)
#define STOP_BIT        (1 << 2)
#define PARITY_EN       (1 << 3)
#define BREAK_TR        (1 << 6)
#define D_LATCH         (1 << 7)

// Fractional divider
#define F_DIVADDVAL     (1 << 0)
#define F_MULVAL        (12 << 4)

void uart0_init(void);
void uart0_byte_transmit(uint8_t data);
void uart0_str_transmit(uint8_t *str);
void uart0_byte_receive(uint8_t *byte);
void uart0_str_receive(uint8_t *str);
void delay_ms(uint32_t millis);
void new_line(void);

#endif
