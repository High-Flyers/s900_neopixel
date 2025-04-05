#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <esp_timer.h>

#define PWM_IN 16

void pwm_enable();
void pwm_disable();

bool pwm_checkTrigger();

uint16_t pwm_getUs();
bool pwm_isAvailable();
