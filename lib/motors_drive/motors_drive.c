#include "motors_drive.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "app.h"
#include "esp_log.h"

#define TAG "DRIVE"

#define MAX_DUTY    1024
#define FADE_TIME   50

esp_err_t motors_drive_init(void)
{
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);
    
    ledc_channel_config_t channel_0 = {
        .gpio_num = TESTING_MOTOR_0_PIN_FORWARD,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_0);

    ledc_channel_config_t channel_1 = {
        .gpio_num = TESTING_MOTOR_0_PIN_BACKWARD,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_1);

    ledc_channel_config_t channel_2 = {
        .gpio_num = TESTING_MOTOR_1_PIN_FORWARD,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_2,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_2);

    ledc_channel_config_t channel_3 = {
        .gpio_num = TESTING_MOTOR_1_PIN_BACKWARD,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_3,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_3);

    ledc_fade_func_install(0);

    return ESP_OK;
}

esp_err_t motors_drive_control_joystick(cJSON *payload)
{
    if (!payload) {
        ESP_LOGE(TAG, "Null JSON payload received");
        return ESP_ERR_INVALID_ARG;
    }
    
    if (!cJSON_IsObject(payload)) {
        ESP_LOGE(TAG, "Invalid JSON payload type");
        return ESP_ERR_INVALID_ARG;
    }
    
    cJSON *x = cJSON_GetObjectItem(payload, "x");
    cJSON *y = cJSON_GetObjectItem(payload, "y");

    float x_val = (x && cJSON_IsNumber(x)) ? (float)x->valuedouble : 0.0f;
    float y_val = (y && cJSON_IsNumber(y)) ? (float)y->valuedouble : 0.0f;

    // Apply arcade drive logic
    float maximum = fmaxf(fabsf(y_val), fabsf(x_val));
    float total = y_val + x_val;
    float difference = y_val - x_val;

    int16_t left_duty, right_duty;

    if (y_val >= 0) {
        if (x_val >= 0) {  // I quadrant
            left_duty = (int16_t)(maximum * MAX_DUTY);
            right_duty = (int16_t)(difference * MAX_DUTY);
        } else {           // II quadrant
            left_duty = (int16_t)(total * MAX_DUTY);
            right_duty = (int16_t)(maximum * MAX_DUTY);
        }
    } else {
        if (x_val >= 0) {  // IV quadrant
            left_duty = (int16_t)(total * MAX_DUTY);
            right_duty = (int16_t)(-maximum * MAX_DUTY);
        } else {           // III quadrant
            left_duty = (int16_t)(-maximum * MAX_DUTY);
            right_duty = (int16_t)(difference * MAX_DUTY);
        }
    }
    
    // Set motor duties for forward and backward directions with fade
    if (left_duty >= 0) {
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, left_duty, FADE_TIME);
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, FADE_TIME);
    } else {
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, FADE_TIME);
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, -left_duty, FADE_TIME);
    }

    if (right_duty >= 0) {
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, right_duty, FADE_TIME);
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 0, FADE_TIME);
    } else {
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, FADE_TIME);
        ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, -right_duty, FADE_TIME);
    }

    ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_NO_WAIT);
    ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, LEDC_FADE_NO_WAIT);
    ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, LEDC_FADE_NO_WAIT);
    ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, LEDC_FADE_NO_WAIT);

    //ESP_LOGI(TAG, "Motor duties - left: %" PRIi16 ", right: %" PRIi16 "", left_duty, right_duty);

    return ESP_OK;
}

esp_err_t motors_drive_testing(void) {
    ESP_LOGI(TAG, "START: Motors testing");
    ESP_LOGI(TAG, "Motor 0 pins: %d, %d", TESTING_MOTOR_0_PIN_FORWARD, TESTING_MOTOR_0_PIN_BACKWARD);
    ESP_LOGI(TAG, "Motor 1 pins: %d, %d", TESTING_MOTOR_1_PIN_FORWARD, TESTING_MOTOR_1_PIN_BACKWARD);
    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 1024, 2500, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 1024, 2500, LEDC_FADE_WAIT_DONE);
    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 2500, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, 2500, LEDC_FADE_WAIT_DONE);

    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 1024, 2500, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 1024, 2500, LEDC_FADE_WAIT_DONE);

    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 2500, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 0, 2500, LEDC_FADE_WAIT_DONE);

    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 1024, 2500, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 1024, 2500, LEDC_FADE_WAIT_DONE);
    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 2500, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 0, 2500, LEDC_FADE_WAIT_DONE);

    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 1024, 2500, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 1024, 2500, LEDC_FADE_WAIT_DONE);

    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 2500, LEDC_FADE_NO_WAIT);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, 2500, LEDC_FADE_WAIT_DONE);

    ESP_LOGI(TAG, "END: Motors testing");
    return ESP_OK;
}


