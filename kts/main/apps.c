#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <inttypes.h>
#include <stdio.h>

#include "include/apps.h"

void button_task(void *arg)
{
    struct state_t *state = (struct state_t *)arg;

    uint32_t io_num;
    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%" PRIu32 "] intr, val: %d\n", io_num,
                   gpio_get_level(io_num));

            if (state->semaphore != NULL) {
                printf("AAA\n");
                if (xSemaphoreTake(state->semaphore, (TickType_t)10)
                    == pdTRUE) {
                    state->mode = gpio_get_level(io_num);
                    xSemaphoreGive(state->semaphore);
                    printf("free A\n");
                }
            }
        }
    }
}
