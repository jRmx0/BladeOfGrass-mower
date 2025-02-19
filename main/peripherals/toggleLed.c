#include "toggleLed.h"
#include <stdio.h>
#include <stdlib.h>
#include "driver/gpio.h"

void led_init(void)
{
  gpio_set_direction(LED, GPIO_MODE_OUTPUT);
}

void led_toggle(bool is_on)
{
  gpio_set_level(LED, is_on);
}