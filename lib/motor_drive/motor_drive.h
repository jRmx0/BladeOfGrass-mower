/**
 * @file motor_drive.h
 * @brief Motor drive control for dual motor system (IBT-2 driver)
 */

 #ifndef MOTOR_DRIVE_H
 #define MOTOR_DRIVE_H
 
 #include "esp_err.h"
 #include "cJSON.h"
 
 // Public APIs
 esp_err_t motor_drive_init(void);
 esp_err_t motor_drive_control_joystick(cJSON *payload);
 esp_err_t motor_drive_test(void);
 
 #endif // MOTOR_DRIVE_H
 