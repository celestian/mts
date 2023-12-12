#include "driver/gpio.h"
#include "esp_log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <inttypes.h>
#include <stdio.h>

static const char *TAG = "kts";
TaskHandle_t myTaskHandle = NULL;

typedef struct diode_state {
    uint pin;
    char name[10];
    char mode;
} diode_state_t;

#define GPIO_OUTPUT_IO_0 CONFIG_GPIO_OUTPUT_0
#define GPIO_OUTPUT_PIN_SEL (1ULL << GPIO_OUTPUT_IO_0)

diode_state_t diode_state = {
    .pin = GPIO_OUTPUT_IO_0,
    .name = "GREEN",
    .mode = 0,
};

void Demo_Task(void *arg)
{

    diode_state_t *diode_state = (diode_state_t *)arg;

    int cnt = 0;
    for (;;) {
        ESP_LOGI(TAG, ":: %i %s", cnt, diode_state->name);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_OUTPUT_IO_0, cnt % 2);
        cnt++;
    }
}

void app_main(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = GPIO_OUTPUT_PIN_SEL,
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);

    printf("velikost: %i", sizeof(diode_state));

    xTaskCreatePinnedToCore(Demo_Task, "Demo_Task", 4096, (void *)&diode_state,
                            10, &myTaskHandle, 1);
}
