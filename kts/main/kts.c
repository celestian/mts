#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <inttypes.h>
#include <stdio.h>

#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "kts";
TaskHandle_t myTaskHandle = NULL;

typedef struct diode_state {
    uint pin;
    char name[10];
    uint32_t mode;
} diode_state_t;

#define GPIO_OUTPUT_IO_0 CONFIG_GPIO_OUTPUT_0
#define GPIO_OUTPUT_PIN_SEL (1ULL << GPIO_OUTPUT_IO_0)

#define GPIO_INPUT_IO_0 CONFIG_GPIO_INPUT_0
#define GPIO_INPUT_PIN_SEL (1ULL << GPIO_INPUT_IO_0)

diode_state_t diode_state = {
    .pin = GPIO_OUTPUT_IO_0,
    .name = "GREEN",
    .mode = 0,
};

#define ESP_INTR_FLAG_DEFAULT 0

static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void *arg)
{
    diode_state_t *diode_state = (diode_state_t *)arg;

    uint32_t io_num;
    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%" PRIu32 "] intr, val: %d\n", io_num,
                   gpio_get_level(io_num));
            diode_state->mode = gpio_get_level(io_num);
        }
    }
}

void Demo_Task(void *arg)
{
    diode_state_t *diode_state = (diode_state_t *)arg;

    for (;;) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_OUTPUT_IO_0, diode_state->mode);
    }
}

void app_main(void)
{
    gpio_config_t led_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = GPIO_OUTPUT_PIN_SEL,
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&led_conf);

    gpio_config_t button_conf = {
        .intr_type = GPIO_INTR_POSEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = GPIO_INPUT_PIN_SEL,
        .pull_down_en = 0,
        .pull_up_en = 1,
    };
    gpio_config(&button_conf);
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    xTaskCreate(gpio_task_example, "gpio_task_example", 2048,
                (void *)&diode_state, 10, NULL);

    xTaskCreatePinnedToCore(Demo_Task, "Demo_Task", 4096, (void *)&diode_state,
                            10, &myTaskHandle, 1);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    // hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler,
                         (void *)GPIO_INPUT_IO_0);

    printf("Minimum free heap size: %" PRIu32 " bytes\n",
           esp_get_minimum_free_heap_size());
}
