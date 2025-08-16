#include <LPC17xx.h>
#include <stdio.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "uart0.h"

// LED connected to P1.19 indicates temperature alert
#define LED_ALERT (1 << 19)  


xTaskHandle tempTaskHandle;
xTaskHandle alertTaskHandle;

// Function declarations
void temp_read_task(void *pvParameters);
void temp_alert_task(void *pvParameters);

// functions to simulate temperature reading 
uint8_t read_temperature(void)
{
    static uint8_t temp = 25;
    temp++;
    if (temp > 40) temp = 25;  
    return temp;
}

int main(void)
{
    // LED pin as output
    LPC_GPIO1->FIODIR |= LED_ALERT;
    LPC_GPIO1->FIOCLR = LED_ALERT;  // LED OFF initially

    // Initialize UART0
    uart0_init();
    uart0_str_transmit("Temperature Monitoring System Start\r\n");

    // Create tasks
    xTaskCreate(temp_read_task, "TEMP_TASK", 100, NULL, 2, &tempTaskHandle);
    xTaskCreate(temp_alert_task, "ALERT_TASK", 100, NULL, 1, &alertTaskHandle);

   
    vTaskStartScheduler();

    while (1);
}

// Task: Read temperature every 1 second
void temp_read_task(void *pvParameters)
{
    uint8_t temperature;
    char buffer[50];

    while (1)
    {
        temperature = read_temperature();
        sprintf(buffer, "Temperature: %d C\r\n", temperature);
        uart0_str_transmit((uint8_t *)buffer);

        // If temperature exceeds threshold, notify alert task
        if (temperature >= 35)
        {
            uart0_str_transmit("High Temperature Alert!\r\n");
            vTaskResume(alertTaskHandle);  // Resume alert LED task
        }

        vTaskDelay((1000 * configTICK_RATE_HZ) / 1000);
    }
}


// Blink LED to indicate alert
void temp_alert_task(void *pvParameters)
{
    
    vTaskSuspend(NULL);

    while (1)
    {
        int i;  
       
        for (i = 0; i < 3; i++)
        {
            LPC_GPIO1->FIOSET = LED_ALERT;
            vTaskDelay((300 * configTICK_RATE_HZ) / 1000);
            LPC_GPIO1->FIOCLR = LED_ALERT;
            vTaskDelay((300 * configTICK_RATE_HZ) / 1000);
        }
        // Suspend itself until next high temperature
        vTaskSuspend(NULL);
    }
}
