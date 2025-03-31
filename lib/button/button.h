#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include "driver/gpio.h"

typedef enum {
    BUTTON_BUILTIN,
    BUTTON_MAX
} button_id_t;

void button_init(button_id_t button, gpio_num_t gpio_num);
void buttons_init(void);

bool button_is_pressed(button_id_t button);
bool button_get_state(button_id_t button);

#endif // BUTTON_H