#include "config_credentials.h"
#include "main.h"

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "led.h"
#include "button.h"
#include "motor_drive.h"
#include "thingsboard.h"
#include "wifi_monitor.h"
#include "uart_um980.h"

#include "nvs_flash.h"
#include "wifi_connect.h"
#include "wifi_led.h"

#include "system.h"

void app_main(void)
{
    system_hardware_init();

    //motor_drive_test();
    
    ESP_ERROR_CHECK(nvs_flash_init());

    wifi_connect_init();
    
    wifi_monitor_init();
    wifi_led_init(3);
    
    ESP_ERROR_CHECK(wifi_connect_sta(WIFI_SSID, WIFI_PASS, 60000));

    tb_init(TB_MQTT_HOST, TB_MQTT_PORT, TB_MQTT_ACCESS_TOKEN);
    tb_connect();

    uart_um980_init();
}


