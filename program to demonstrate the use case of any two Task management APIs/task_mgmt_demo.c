#include <LPC17xx.h> 
#include <stdio.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "uart0.h"

// LED1 is connected to port 1, pin 19
#define LED1 (1 << 19)  

// Task handles 
xTaskHandle ledTaskHandle;
xTaskHandle printTaskHandle;

// Function declarations
void led_task(void *pvParameters);
void print_task(void *pvParameters);

int main(void)
{
    // Set LED1 pin as output
    LPC_GPIO1->FIODIR |= LED1;
    LPC_GPIO1->FIOCLR = LED1; // Start with LED off

    // Setup UART0 to send message to pc
    uart0_init();
    uart0_str_transmit("Suspend & Resume API Demo\r\n");

    // Task to blink the LED
    xTaskCreate(led_task, "LED_TASK", 100, NULL, 1, &ledTaskHandle);

    // Task to print messages on UART
    xTaskCreate(print_task, "PRINT_TASK", 100, NULL, 1, &printTaskHandle);

    // Scheduler (starts running tasks)
    vTaskStartScheduler();

    while (1);
}

// Blinks the LED and also controls the print task
void led_task(void *pvParameters)
{
    int count = 0;

    while (1)
    {
        // Turn LED ON
        LPC_GPIO1->FIOSET = LED1;
        vTaskDelay((500 * configTICK_RATE_HZ) / 1000); // Wait 500ms

        // Turn LED OFF
        LPC_GPIO1->FIOCLR = LED1;
        vTaskDelay((500 * configTICK_RATE_HZ) / 1000); // Wait 500ms

        count++; 

        // After 5 blinks, stop the print task for 3 seconds
        if (count == 5)
        {
            uart0_str_transmit("Suspending print task...\r\n");
            vTaskSuspend(printTaskHandle);
            vTaskDelay((3000 * configTICK_RATE_HZ) / 1000); 
            uart0_str_transmit("Resuming print task...\r\n");
            vTaskResume(printTaskHandle); 
        }
    }
}

// Task sends a message every 1 second
void print_task(void *pvParameters)
{
    int msgCount = 0;
    char buffer[40];

    while (1)
    {
        // Create message with counter 
        sprintf(buffer, "Print Task Count: %d\r\n", msgCount++);

        // Send the message over UART
        uart0_str_transmit((uint8_t *)buffer);

        // Wait for 1 second before printing again
        vTaskDelay((1000 * configTICK_RATE_HZ) / 1000);
    }
}

