@echo off
cd /D C:\Users\justa\Documents\PlatformIO\Projects\mower-port-v0.0.2\build\esp-idf\esp_system || (set FAIL_LINE=2& goto :ABORT)
c:\Espressif\v5.4.0\tools\python_env\idf5.4_py3.11_env\Scripts\python.exe C:/Espressif/v5.4.0/esp-idf/tools/ldgen/ldgen.py --config C:/Users/justa/Documents/PlatformIO/Projects/mower-port-v0.0.2/sdkconfig --fragments-list C:/Espressif/v5.4.0/esp-idf/components/xtensa/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_gpio/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_pm/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_mm/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/spi_flash/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_system/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_system/app.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_common/common.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_common/soc.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_rom/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/hal/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/log/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/heap/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/soc/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_hw_support/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_hw_support/dma/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_hw_support/ldo/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/freertos/linker_common.lf;C:/Espressif/v5.4.0/esp-idf/components/freertos/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/newlib/newlib.lf;C:/Espressif/v5.4.0/esp-idf/components/newlib/system_libs.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_gptimer/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_ringbuf/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_uart/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/app_trace/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_event/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_pcnt/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_spi/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_mcpwm/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_ana_cmpr/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_dac/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_rmt/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_sdm/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_i2c/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_ledc/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_parlio/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_usb_serial_jtag/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/driver/twai/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_phy/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/vfs/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/lwip/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_netif/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/wpa_supplicant/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_wifi/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_adc/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_driver_isp/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_eth/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_gdbstub/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_psram/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/esp_lcd/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/espcoredump/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/ieee802154/linker.lf;C:/Espressif/v5.4.0/esp-idf/components/openthread/linker.lf --input C:/Users/justa/Documents/PlatformIO/Projects/mower-port-v0.0.2/build/esp-idf/esp_system/ld/sections.ld.in --output C:/Users/justa/Documents/PlatformIO/Projects/mower-port-v0.0.2/build/esp-idf/esp_system/ld/sections.ld --kconfig C:/Espressif/v5.4.0/esp-idf/Kconfig --env-file C:/Users/justa/Documents/PlatformIO/Projects/mower-port-v0.0.2/build/config.env --libraries-file C:/Users/justa/Documents/PlatformIO/Projects/mower-port-v0.0.2/build/ldgen_libraries --objdump C:/Espressif/v5.4.0/tools/tools/xtensa-esp-elf/esp-14.2.0_20241119/xtensa-esp-elf/bin/xtensa-esp32-elf-objdump.exe || (set FAIL_LINE=3& goto :ABORT)
goto :EOF

:ABORT
set ERROR_CODE=%ERRORLEVEL%
echo Batch file failed at line %FAIL_LINE% with errorcode %ERRORLEVEL%
exit /b %ERROR_CODE%