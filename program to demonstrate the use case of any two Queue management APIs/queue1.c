#include <LPC17xx.h>
#include <stdio.h>  // for sprintf
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart0.h"

#define LED1 (1 << 19)  // LED on P1.19
#define QUEUE_SIZE 3

xQueueHandle myQueue;  // Queue to store numbers

void producer(void *pvParameters);
void consumer(void *pvParameters);

int main(void)
{
    LPC_GPIO1->FIODIR |= LED1;  // LED output
    LPC_GPIO1->FIOCLR = LED1;

    uart0_init();
    uart0_str_transmit("Simple Queue Demo\r\n");

    myQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
    if (myQueue == NULL) while(1);

    xTaskCreate(producer, "PRODUCER", 100, NULL, 1, NULL);
    xTaskCreate(consumer, "CONSUMER", 100, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1);
}

// Produce numbers and add to queue
void producer(void *pvParameters)
{
    int num = 0;
    int freeSlots;
    char msg[50];

    while(1)
    {
        num++;
        xQueueSendToBack(myQueue, &num, portMAX_DELAY);

        freeSlots = QUEUE_SIZE - uxQueueMessagesWaiting(myQueue);
        sprintf(msg, "Added: %d | Free slots: %d\r\n", num, freeSlots);
        uart0_str_transmit((uint8_t *)msg);

        vTaskDelay(1000);
    }
}

// Consume numbers from queue and blink LED
void consumer(void *pvParameters)
{
    int num;
    char msg[50];

    while(1)
    {
        if (uxQueueMessagesWaiting(myQueue) > 0)
        {
            xQueueReceive(myQueue, &num, 0);

            sprintf(msg, "Got: %d\r\n", num);
            uart0_str_transmit((uint8_t *)msg);

            LPC_GPIO1->FIOSET = LED1;
            vTaskDelay(200);
            LPC_GPIO1->FIOCLR = LED1;
        }
        vTaskDelay(500);
    }
}

