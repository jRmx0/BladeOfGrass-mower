# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

idf_component_register(
    SRCS 
        ${app_sources}
    PRIV_REQUIRES 
        spi_flash
        nvs_flash
        # wifi_connect
        esp_http_server
        esp_driver_gpio
        json
        fatfs          
    REQUIRES
        esp_driver_ledc
)

fatfs_create_rawflash_image(storage ../site/dist FLASH_IN_PROJECT)