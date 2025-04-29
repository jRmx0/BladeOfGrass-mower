#include "config_board.h"
#include "led.h"

#include "driver/gpio.h"
#include <stdio.h>
#include "esp_log.h"

#define TAG "LED"

static gpio_num_t led_gpio[LED_MAX];
static bool led_state[LED_MAX];

void led_init(led_id_t led, gpio_num_t gpio_num)
{
    if (led >= LED_MAX) 
    {
        ESP_LOGE(TAG, "Invalid led init, LED ID: %d", led);
        return;
    }

    led_gpio[led] = gpio_num;
    led_state[led] = false;

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << gpio_num),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
    gpio_set_level(gpio_num, 0);
}

void led_on(led_id_t led)
{
    if (led >= LED_MAX) 
    {
        ESP_LOGE(TAG, "Invalid led on, LED ID: %d", led);
        return;
    }
    gpio_set_level(led_gpio[led], 1);
    led_state[led] = true;
}

void led_off(led_id_t led)
{
    if (led >= LED_MAX) 
    {
        ESP_LOGE(TAG, "Invalid led off, LED ID: %d", led);
        return;
    }
    gpio_set_level(led_gpio[led], 0);
    led_state[led] = false;
}

void led_toggle(led_id_t led)
{
    if (led >= LED_MAX) 
    {
        ESP_LOGE(TAG, "Invalid led toggle, LED ID: %d", led);
        return;
    }
    bool new_state = !led_state[led];
    gpio_set_level(led_gpio[led], new_state);
    led_state[led] = new_state;
}

void led_set_state(led_id_t led, bool is_on)
{
    if (led >= LED_MAX) 
    {
        ESP_LOGE(TAG, "Invalid led set, LED ID: %d", led);
        return;
    }
    gpio_set_level(led_gpio[led], is_on);
    led_state[led] = is_on;
}

bool led_get_state(led_id_t led)
{
    if (led >= LED_MAX) 
    {
        ESP_LOGE(TAG, "Invalid led get, LED ID: %d", led);
        return 0;
    }
    return led_state[led];
}

void leds_init(void)
{
    led_init(LED_BUILTIN, BOARD_LED_BUILTIN_GPIO);
}