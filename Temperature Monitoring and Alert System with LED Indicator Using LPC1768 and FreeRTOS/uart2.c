#include "uart0.h"

void uart0_init(void)
{
    LPC_SC->PCONP |= UART0_PWR;             // Power-on 
    LPC_SC->CLKSRCSEL = RC_OSC_CLK;         // Internal RC 
    LPC_SC->PCLKSEL0 &= ~CCLKDIV;           // PCLK = CCLK/4

    // Configure P0.2 as TXD0
    LPC_PINCON->PINSEL0 |=  (1 << 4);
    LPC_PINCON->PINSEL0 &= ~(1 << 5);

    // Configure P0.3 as RXD0
    LPC_PINCON->PINSEL0 |=  (1 << 6);
    LPC_PINCON->PINSEL0 &= ~(1 << 7);

    LPC_UART0->LCR |= DATA_LEN;                      
    LPC_UART0->LCR &= ~(BREAK_TR | PARITY_EN | STOP_BIT); 
    LPC_UART0->LCR |= D_LATCH;                       

    LPC_UART0->DLL = 0x06;  
    LPC_UART0->DLM = 0x00;
    LPC_UART0->FDR = (F_DIVADDVAL | F_MULVAL);

    LPC_UART0->LCR &= ~D_LATCH;                      
}

void uart0_byte_transmit(uint8_t data)
{
    LPC_UART0->THR = data;
    while ((LPC_UART0->LSR & (1 << 5)) == 0);
}

void uart0_str_transmit(uint8_t *str)
{
    while (*str != '\0')
    {
        uart0_byte_transmit(*str++);
    }
}

void uart0_byte_receive(uint8_t *byte)
{
    while ((LPC_UART0->LSR & (1 << 0)) == 0);
    *byte = LPC_UART0->RBR;
}

void uart0_str_receive(uint8_t *str)
{
    while (1)
    {
        while ((LPC_UART0->LSR & (1 << 0)) == 0);
        *str = LPC_UART0->RBR;
        if (*str == '\0') break;
        str++;
    }
}

void delay_ms(uint32_t millis)
{
    uint32_t i, j;
    for (i = 0; i < millis; i++)
        for (j = 0; j < 6000; j++);
}

void new_line(void)
{
    uart0_byte_transmit('\r');
    uart0_byte_transmit('\n');
}
