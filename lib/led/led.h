#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include <stdint.h>
#include "driver/gpio.h"

typedef enum {
    LED_BUILTIN,
    LED_MAX
} led_id_t;

void led_init(led_id_t led, gpio_num_t gpio_num);

void led_on(led_id_t led);
void led_off(led_id_t led);
void led_toggle(led_id_t led);
void led_set_state(led_id_t led, bool is_on);
bool led_get_state(led_id_t led); 

void leds_init(void);

#endif // LED_H