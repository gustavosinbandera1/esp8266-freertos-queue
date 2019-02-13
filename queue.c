/* The classic "blink" example
 *
 * This sample code is in the public domain.
 */
#include <stdlib.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h" 
#include "queue.h"


const int gpio = 2;

/* This task uses the high level GPIO API (esp_gpio.h) to blink an LED.
 *
 */
void task1(void *pvParameters)
{
   QueueHandle_t *queue = (QueueHandle_t *)pvParameters;
   printf('Hello from Task1!\r\n');
   u_int32_t count = 0;
   while(1) {
       vTaskDelay(100);
       xQueueSend(*queue, &count, 0);
       count++;
       printf("Count productor %u",count);
   }
    
}

void task2(void *pvParameters)
{
   printf('hELLO WORLD FROM TASK2!\r\n');
   QueueHandle_t *queue = (QueueHandle_t *)pvParameters;
   while(1) {
       uint32_t count;
       if(xQueueReceive(*queue, &count, 1000)) {
           printf("Got %u\n", count);
       }else {
           printf("no messages ):\n");
       }
   }
    
}

static QueueHandle_t mainqueue;

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK Version: %s\n", sdk_system_get_sdk_version());

    mainqueue = xQueueCreate(10, sizeof(uint_least32_t));

    xTaskCreate(task1, "tsk1", 256, &mainqueue, 2, NULL);
    xTaskCreate(task2, "tsk2", 256, &mainqueue, 2, NULL);
}
