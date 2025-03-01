#include "main.h"

void app_main(void)
{
    motors_drive_init();
    led_init();
    btn_init();

    //motors_drive_testing();
    
    ESP_ERROR_CHECK(nvs_flash_init());
    
    wifi_connect_init();
    ESP_ERROR_CHECK(wifi_connect_sta(WIFI_SSID, WIFI_PASS, 10000));

    //start_mdns_service();
    mount_fs();
    init_server();
}
