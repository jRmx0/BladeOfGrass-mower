#include "main.h"

void app_main(void)
{
    //testing_motors();
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_connect_init();
    ESP_ERROR_CHECK(wifi_connect_sta(WIFI_SSID, WIFI_PASS, 10000));
}
