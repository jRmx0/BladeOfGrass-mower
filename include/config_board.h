#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "driver/gpio.h"

// === LEDS ===
#define BOARD_LED_BUILTIN_GPIO              GPIO_NUM_39
#define LED_TESTING                         GPIO_NUM_39

// === BUTTONS ===
#define BOARD_BUTTON_BUILTIN_GPIO           GPIO_NUM_0

// === MOTORS ===
#define BOARD_MOTOR_0_PIN_FORWARD           6
#define BOARD_MOTOR_0_PIN_BACKWARD          7
#define BOARD_MOTOR_1_PIN_FORWARD           42
#define BOARD_MOTOR_1_PIN_BACKWARD          41

// UART UM980
#define UART_TX_PIN 17
#define UART_RX_PIN 18

#endif // BOARD_CONFIG_H
