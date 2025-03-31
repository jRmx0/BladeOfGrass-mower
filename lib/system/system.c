#include "system.h"

#include "led.h"
#include "button.h"
#include "motor_drive.h"

void system_hardware_init(void)
{
    leds_init();
    buttons_init();
    motor_drive_init();
}