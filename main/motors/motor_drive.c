#include "motor_drive.h"

#define TAG "DRIVE"

esp_err_t testing_motors () {
    ESP_LOGI(TAG, "START: Motors testing");
    ESP_LOGI(TAG, "Motor 0 pins: %d, %d", TESTING_MOTOR_0_PIN_0, TESTING_MOTOR_0_PIN_1);
    ESP_LOGI(TAG, "Motor 1 pins: %d, %d", TESTING_MOTOR_1_PIN_0, TESTING_MOTOR_1_PIN_1);
    
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);
    
    ledc_channel_config_t channel_0 = {
        .gpio_num = TESTING_MOTOR_0_PIN_0,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_0);

    ledc_channel_config_t channel_1 = {
        .gpio_num = TESTING_MOTOR_0_PIN_1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_1);

    ledc_channel_config_t channel_2 = {
        .gpio_num = TESTING_MOTOR_1_PIN_0,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_2,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_2);

    ledc_channel_config_t channel_3 = {
        .gpio_num = TESTING_MOTOR_1_PIN_1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_3,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_3);

    ledc_fade_func_install(0);

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


