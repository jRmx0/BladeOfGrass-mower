// For IBT-2 motor driver module 

#ifndef MOTOR_DRIVE_H
#define MOTOR_DRIVE_H

#define TESTING_MOTOR_0_PIN_FORWARD     6
#define TESTING_MOTOR_0_PIN_BACKWARD    7
#define TESTING_MOTOR_1_PIN_FORWARD     42
#define TESTING_MOTOR_1_PIN_BACKWARD    41

#include "esp_err.h"
#include "cJSON.h"

esp_err_t motors_drive_init(void);
esp_err_t motors_drive_control_joystick(cJSON *payload);
esp_err_t motors_drive_testing(void);

#endif
