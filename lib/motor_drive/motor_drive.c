#include "config_board.h"
#include "motor_drive.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"

#define TAG        "MOTOR_DRIVE"
#define MAX_DUTY   1024
#define FADE_TIME  50

esp_err_t motor_drive_init(void)
{
    ESP_LOGI(TAG, "Initializing motor drive...");

    ledc_timer_config_t timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 5000,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    // Motor 0 (Left)
    ledc_channel_config_t motor0_forward = {
        .gpio_num   = BOARD_MOTOR_0_PIN_FORWARD,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_0,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
        .hpoint     = 0
    };
    ledc_channel_config(&motor0_forward);

    ledc_channel_config_t motor0_backward = {
        .gpio_num   = BOARD_MOTOR_0_PIN_BACKWARD,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_1,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
        .hpoint     = 0
    };
    ledc_channel_config(&motor0_backward);

    // Motor 1 (Right)
    ledc_channel_config_t motor1_forward = {
        .gpio_num   = BOARD_MOTOR_1_PIN_FORWARD,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_2,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
        .hpoint     = 0
    };
    ledc_channel_config(&motor1_forward);

    ledc_channel_config_t motor1_backward = {
        .gpio_num   = BOARD_MOTOR_1_PIN_BACKWARD,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_3,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
        .hpoint     = 0
    };
    ledc_channel_config(&motor1_backward);

    ledc_fade_func_install(0);

    ESP_LOGI(TAG, "Motor drive initialized");
    return ESP_OK;
}

esp_err_t motor_drive_control_joystick(cJSON *payload)
{
    if (!payload || !cJSON_IsObject(payload)) {
        ESP_LOGE(TAG, "Invalid joystick payload");
        return ESP_ERR_INVALID_ARG;
    }

    cJSON *x = cJSON_GetObjectItem(payload, "x");
    cJSON *y = cJSON_GetObjectItem(payload, "y");

    float x_val = (x && cJSON_IsNumber(x)) ? (float)x->valuedouble : 0.0f;
    float y_val = (y && cJSON_IsNumber(y)) ? (float)y->valuedouble : 0.0f;

    // Arcade drive logic
    float maximum    = fmaxf(fabsf(y_val), fabsf(x_val));
    float total      = y_val + x_val;
    float difference = y_val - x_val;

    int16_t left_duty = 0;
    int16_t right_duty = 0;

    if (y_val >= 0) {
        if (x_val >= 0) {  // I quadrant
            left_duty  = (int16_t)(maximum * MAX_DUTY);
            right_duty = (int16_t)(difference * MAX_DUTY);
        } else {           // II quadrant
            left_duty  = (int16_t)(total * MAX_DUTY);
            right_duty = (int16_t)(maximum * MAX_DUTY);
        }
    } else {
        if (x_val >= 0) {  // IV quadrant
            left_duty  = (int16_t)(total * MAX_DUTY);
            right_duty = (int16_t)(-maximum * MAX_DUTY);
        } else {           // III quadrant
            left_duty  = (int16_t)(-maximum * MAX_DUTY);
            right_duty = (int16_t)(difference * MAX_DUTY);
        }
    }

    // Apply PWM fade to the motors
    // Left motor
    if (left_duty >= 0) {
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, left_duty, FADE_TIME);
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, FADE_TIME);
    } else {
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, FADE_TIME);
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, -left_duty, FADE_TIME);
    }

    // Right motor
    if (right_duty >= 0) {
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, right_duty, FADE_TIME);
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 0, FADE_TIME);
    } else {
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, FADE_TIME);
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, -right_duty, FADE_TIME);
    }

    // Start fades
    ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_NO_WAIT);
    ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, LEDC_FADE_NO_WAIT);
    ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, LEDC_FADE_NO_WAIT);
    ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, LEDC_FADE_NO_WAIT);

    ESP_LOGD(TAG, "Left: %d | Right: %d", left_duty, right_duty);
    return ESP_OK;
}

esp_err_t motor_drive_test(void)
{
    ESP_LOGI(TAG, "=== Motor drive test started ===");

    ESP_LOGI(TAG, "Motor 0: %d (FWD), %d (BWD)", BOARD_MOTOR_0_PIN_FORWARD, BOARD_MOTOR_0_PIN_BACKWARD);
    ESP_LOGI(TAG, "Motor 1: %d (FWD), %d (BWD)", BOARD_MOTOR_1_PIN_FORWARD, BOARD_MOTOR_1_PIN_BACKWARD);

    const int duty = MAX_DUTY;
    const int delay = 2500;

    // Example sequence - Forward motors
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty, delay, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, duty, delay, LEDC_FADE_WAIT_DONE);

    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, delay, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, delay, LEDC_FADE_WAIT_DONE);

    // Reverse motors
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty, delay, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, duty, delay, LEDC_FADE_WAIT_DONE);

    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, delay, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 0, delay, LEDC_FADE_WAIT_DONE);

    ESP_LOGI(TAG, "=== Motor drive test completed ===");

    return ESP_OK;
}
