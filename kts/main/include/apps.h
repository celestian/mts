#pragma once

#include <freertos/semphr.h>

struct state_t {
    uint pin;
    char name[10];
    uint32_t mode;
    SemaphoreHandle_t semaphore;
};

static QueueHandle_t gpio_evt_queue = NULL;

void button_task(void *arg);
