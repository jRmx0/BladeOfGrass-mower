#include "toggleLed.h" // tested

#include "motors_drive.h" // tested
#include "cJSON.h"

#include "server.h" // tested
#include "esp_http_server.h"
#include "wifi_credentials.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "main.h"

#include "unity.h" // testing framework

void setUp(void)
{
    gpio_reset_pin(LED);
    motors_drive_init();
}

void tearDown(void)
{
    gpio_reset_pin(LED);
}

// LED testing
void test_function_led_toggle(void)
{
    gpio_set_direction(LED, GPIO_MODE_INPUT_OUTPUT);
    led_toggle(1);
    TEST_ASSERT_EQUAL(1, gpio_get_level(LED));

    led_toggle(0);
    TEST_ASSERT_EQUAL(0, gpio_get_level(LED));
}

// Motors control testing
void test_motors_drive_null_payload(void)
{
    esp_err_t result = motors_drive_control_joystick(NULL);
    TEST_ASSERT_EQUAL(ESP_ERR_INVALID_ARG, result);
}

void test_motors_drive_invalid_payload_type(void)
{
    cJSON *array = cJSON_CreateArray();
    esp_err_t result = motors_drive_control_joystick(array);
    TEST_ASSERT_EQUAL(ESP_ERR_INVALID_ARG, result);
    cJSON_Delete(array);
}

void test_motors_drive_forward(void)
{
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddNumberToObject(payload, "x", 0.0);
    cJSON_AddNumberToObject(payload, "y", 1.0);
    
    esp_err_t result = motors_drive_control_joystick(payload);
    
    vTaskDelay(pdMS_TO_TICKS(100));

    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(1024, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0)); // Left forward at max
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1));    // Left backward off
    TEST_ASSERT_EQUAL(1024, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2)); // Right forward at max
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3));    // Right backward off
    
    cJSON_Delete(payload);
}

void test_motors_drive_backward(void)
{
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddNumberToObject(payload, "x", 0.0);
    cJSON_AddNumberToObject(payload, "y", -1.0);
    
    esp_err_t result = motors_drive_control_joystick(payload);
    
    vTaskDelay(pdMS_TO_TICKS(100));

    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));    // Left forward off
    TEST_ASSERT_EQUAL(1024, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1)); // Left backward at max
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2));    // Right forward off
    TEST_ASSERT_EQUAL(1024, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3)); // Right backward at max
    
    cJSON_Delete(payload);
}

void test_motors_drive_right_turn(void)
{
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddNumberToObject(payload, "x", 1.0);
    cJSON_AddNumberToObject(payload, "y", 0.0);
    
    esp_err_t result = motors_drive_control_joystick(payload);
    
    vTaskDelay(pdMS_TO_TICKS(100));

    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(1024, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0)); // Left forward at max
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1));    // Left backward off
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2));    // Right forward off
    TEST_ASSERT_EQUAL(1024, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3)); // Right backward at max
    
    cJSON_Delete(payload);
}

void test_motors_drive_left_turn(void)
{
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddNumberToObject(payload, "x", -1.0);
    cJSON_AddNumberToObject(payload, "y", 0.0);
    
    esp_err_t result = motors_drive_control_joystick(payload);
    
    vTaskDelay(pdMS_TO_TICKS(100));

    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));    // Left forward off
    TEST_ASSERT_EQUAL(1024, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1)); // Left backward at max
    TEST_ASSERT_EQUAL(1024, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2)); // Right forward at max
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3));    // Right backward off
    
    cJSON_Delete(payload);
}

void test_motors_drive_forward_right(void)
{
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddNumberToObject(payload, "x", 0.7);
    cJSON_AddNumberToObject(payload, "y", 0.7);
    
    esp_err_t result = motors_drive_control_joystick(payload);
    
    vTaskDelay(pdMS_TO_TICKS(100));

    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(716, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0)); // Left forward at 70%
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1));   // Left backward off
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2));   // Right forward off
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3));   // Right backward off
    
    cJSON_Delete(payload);
}

void test_motors_drive_forward_left(void)
{
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddNumberToObject(payload, "x", -0.7);
    cJSON_AddNumberToObject(payload, "y", 0.7);
    
    esp_err_t result = motors_drive_control_joystick(payload);
    
    vTaskDelay(pdMS_TO_TICKS(100));

    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));   // Left forward off
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1));   // Left backward off
    TEST_ASSERT_EQUAL(716, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2)); // Right forward at 70%
    TEST_ASSERT_EQUAL(0, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3));   // Right backward off
    
    cJSON_Delete(payload);
}

// Wifi connection testing
// Mount filesystem
void test_mount_fs(void)
{
    motors_drive_init();
    led_init();
    btn_init();
    
    ESP_ERROR_CHECK(nvs_flash_init());

    mount_fs();
    TEST_PASS();
}

// Test basic WiFi connection with valid credentials
void test_wifi_connect_valid_credentials(void)
{
    wifi_connect_init();

    esp_err_t result = wifi_connect_sta(WIFI_SSID, WIFI_PASS, 10000);
    TEST_ASSERT_EQUAL(ESP_OK, result);

    wifi_disconnect();
}

void test_wifi_connect_invalid_credentials(void)
{
    esp_err_t fail_result = wifi_connect_sta("invalid_ssid", "wrong_password", 5000);
    TEST_ASSERT_EQUAL(ESP_FAIL, fail_result);
   
    wifi_disconnect();
}

void test_wifi_connect_timeout_handling(void)
{
    esp_err_t timeout_result = wifi_connect_sta(WIFI_SSID, WIFI_PASS, 10);
    TEST_ASSERT_EQUAL(ESP_FAIL, timeout_result);
    
    wifi_disconnect();
}

void test_wifi_disconnect_function(void)
{
    esp_err_t result = wifi_connect_sta(WIFI_SSID, WIFI_PASS, 10000);
    TEST_ASSERT_EQUAL(ESP_OK, result);
    
    wifi_disconnect();
    
    esp_err_t second_result = wifi_connect_sta(WIFI_SSID, WIFI_PASS, 10000);
    TEST_ASSERT_EQUAL(ESP_OK, second_result);
    
    wifi_disconnect();
}

void test_init_server(void)
{
    ESP_ERROR_CHECK(wifi_connect_sta(WIFI_SSID, WIFI_PASS, 10000));

    init_server();
    TEST_PASS();
}

void test_log_heap_usage(void)
{
    // Checks if log_heap_usage() runs without crashing
    log_heap_usage();
    TEST_PASS();
}

void test_send_ws_message_no_client(void)
{
    esp_err_t result = send_ws_message("test message");
    
    TEST_ASSERT_EQUAL(-1, result);
}

// Test parsing JSON payload for toggle LED
void test_toggle_led_json_parsing(void)
{
    gpio_set_direction(LED, GPIO_MODE_INPUT_OUTPUT);
    
    const char *json_str = "{\"is_on\":true}";
    cJSON *payload = cJSON_Parse(json_str);
    cJSON *is_on_json = cJSON_GetObjectItem(payload, "is_on");
    bool is_on = cJSON_IsTrue(is_on_json);
    
    led_toggle(is_on);
    
    TEST_ASSERT_TRUE(gpio_get_level(LED));
    cJSON_Delete(payload);
    
    // Test false state
    json_str = "{\"is_on\":false}";
    payload = cJSON_Parse(json_str);
    is_on_json = cJSON_GetObjectItem(payload, "is_on");
    is_on = cJSON_IsTrue(is_on_json);
    
    led_toggle(is_on);
    
    TEST_ASSERT_FALSE(gpio_get_level(LED));
    cJSON_Delete(payload);
}

void test_file_extension_detection(void)
{
    const char *css_uri = "/styles.css";
    const char *js_uri = "/script.js";
    const char *png_uri = "/image.png";
    const char *html_uri = "/page.html";
    const char *no_ext_uri = "/noextension";
    
    char *ext;
    
    ext = strrchr(css_uri, '.');
    TEST_ASSERT_NOT_NULL(ext);
    TEST_ASSERT_EQUAL_STRING(".css", ext);
    
    ext = strrchr(js_uri, '.');
    TEST_ASSERT_NOT_NULL(ext);
    TEST_ASSERT_EQUAL_STRING(".js", ext);
    
    ext = strrchr(png_uri, '.');
    TEST_ASSERT_NOT_NULL(ext);
    TEST_ASSERT_EQUAL_STRING(".png", ext);
    
    ext = strrchr(html_uri, '.');
    TEST_ASSERT_NOT_NULL(ext);
    TEST_ASSERT_EQUAL_STRING(".html", ext);
    
    ext = strrchr(no_ext_uri, '.');
    TEST_ASSERT_NULL(ext);
}

void test_websocket_message_creation(void)
{
    char *message = "test message";
    
    httpd_ws_frame_t ws_message = {
        .final = true,
        .fragmented = false,
        .len = strlen(message),
        .payload = (uint8_t *)message,
        .type = HTTPD_WS_TYPE_TEXT
    };
    
    TEST_ASSERT_TRUE(ws_message.final);
    TEST_ASSERT_FALSE(ws_message.fragmented);
    TEST_ASSERT_EQUAL(12, ws_message.len);
    TEST_ASSERT_EQUAL_STRING("test message", (char*)ws_message.payload);
    TEST_ASSERT_EQUAL(HTTPD_WS_TYPE_TEXT, ws_message.type);
}

// Running tests
void app_main(void)
{
    UNITY_BEGIN();
    
    RUN_TEST(test_function_led_toggle);
    
    RUN_TEST(test_motors_drive_null_payload);
    RUN_TEST(test_motors_drive_invalid_payload_type);
    RUN_TEST(test_motors_drive_forward);
    RUN_TEST(test_motors_drive_backward);
    RUN_TEST(test_motors_drive_right_turn);
    RUN_TEST(test_motors_drive_left_turn);
    RUN_TEST(test_motors_drive_forward_right);
    RUN_TEST(test_motors_drive_forward_left);
    
    RUN_TEST(test_mount_fs);

    RUN_TEST(test_wifi_connect_valid_credentials);
    RUN_TEST(test_wifi_connect_invalid_credentials);
    RUN_TEST(test_wifi_connect_timeout_handling);
    RUN_TEST(test_wifi_disconnect_function);

    RUN_TEST(test_init_server);

    RUN_TEST(test_log_heap_usage);
    RUN_TEST(test_send_ws_message_no_client);
    RUN_TEST(test_toggle_led_json_parsing);
    RUN_TEST(test_file_extension_detection);
    RUN_TEST(test_websocket_message_creation);

    UNITY_END();
}
