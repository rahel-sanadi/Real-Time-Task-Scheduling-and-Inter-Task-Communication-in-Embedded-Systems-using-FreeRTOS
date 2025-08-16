#include <LPC17xx.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "uart0.h"

#define LED1 (1 << 19)  // LED on P1.19

void led_task(void *pvParameters);
void msg_task(void *pvParameters);

int main(void)
{
    // Configure LED as output
    LPC_GPIO1->FIODIR |= LED1;
    LPC_GPIO1->FIOCLR = LED1;

    // Initialize UART
    uart0_init();
    uart0_str_transmit("Simulated Software Timer Demo\r\n");

    // Create LED task
    xTaskCreate(led_task, "LED_TASK", 100, NULL, 1, NULL);

    // Create message task
    xTaskCreate(msg_task, "MSG_TASK", 100, NULL, 1, NULL);

    
    vTaskStartScheduler();

    while(1);  
}

// LED blink task
void led_task(void *pvParameters)
{
    while(1)
    {
        LPC_GPIO1->FIOPIN ^= LED1;  // Toggle LED
        vTaskDelay(500 / portTICK_RATE_MS);  // Delay 500 ms
    }
}

// Message task
void msg_task(void *pvParameters)
{
    int counter = 0;      
    char buffer[50];     

    while(1)
    {
        counter++;
        sprintf(buffer, "Message count: %d\r\n", counter);
        uart0_str_transmit((uint8_t *)buffer);

        if(counter == 5)
        {
            uart0_str_transmit("LED will blink faster now\r\n");
        }

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

