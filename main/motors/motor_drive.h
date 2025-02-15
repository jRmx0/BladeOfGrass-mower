#ifndef MOTOR_DRIVE_H
#define MOTOR_DRIVE_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"

#define TESTING_MOTOR_0_PIN_0 6
#define TESTING_MOTOR_0_PIN_1 7
#define TESTING_MOTOR_1_PIN_0 42
#define TESTING_MOTOR_1_PIN_1 41

esp_err_t testing_motors();

#endif
