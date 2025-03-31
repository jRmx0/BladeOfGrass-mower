#include "config_credentials.h"
#include "main.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "led.h"
#include "button.h"
#include "motor_drive.h"
#include "thingsboard.h"

#include "nvs_flash.h"
#include "wifi_connect.h"

#include "system.h"

void app_main(void)
{
    system_hardware_init();

    //motor_drive_test();
    
    ESP_ERROR_CHECK(nvs_flash_init());
    
    wifi_connect_init();
    ESP_ERROR_CHECK(wifi_connect_sta(WIFI_SSID, WIFI_PASS, 60000));

    // Initialize ThingsBoard
    tb_init(TB_MQTT_HOST, TB_MQTT_PORT, TB_MQTT_ACCESS_TOKEN);
    tb_register_rpc_callback(tb_default_rpc_handler);
    tb_connect();
}
