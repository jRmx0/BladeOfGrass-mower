#include "main.h"

void app_main(void)
{
    init_motors();
    testing_motors();
    
    ESP_ERROR_CHECK(nvs_flash_init());
    init_led();
    init_btn();
    wifi_connect_init();
    ESP_ERROR_CHECK(wifi_connect_sta(WIFI_SSID, WIFI_PASS, 10000));

    start_mdns_service();
    mount_fs();
    init_server();
}
