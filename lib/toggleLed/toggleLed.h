#ifndef toggleLed_h
#define toggleLed_h

#include <stdbool.h>

#define LED 38

void led_init(void);
void led_toggle(bool is_on);

#endif