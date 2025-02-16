idf_component_register(
    SRCS 
        "main.c" 
        "motors/motor_drive.c"
        "communication/server.c"
        "peripherals/pushBtn.c"
        "peripherals/toggleLed.c"
    PRIV_REQUIRES 
        spi_flash
        nvs_flash
        wifi_connect
        esp_http_server
        esp_driver_gpio
        json
        fatfs                
    REQUIRES
        esp_driver_ledc
    INCLUDE_DIRS 
        "" 
        "motors/"
        "communication/"
        "peripherals/"
)

fatfs_create_rawflash_image(storage ../site/dist FLASH_IN_PROJECT)