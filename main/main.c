#include "main.h"

void app_main(void)
{
    init_motors();
    init_led();
    init_btn();
    
    //testing_motors();
    
    ESP_ERROR_CHECK(nvs_flash_init());
    
    wifi_connect_init();
    ESP_ERROR_CHECK(wifi_connect_sta(WIFI_SSID, WIFI_PASS, 10000));

    start_mdns_service();
    mount_fs();
    init_server();
}
