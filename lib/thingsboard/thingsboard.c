// TODO:
// - Sending location data to TB

#include "config_board.h"

#include "mqtt_client.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <driver/gpio.h>

#include "led.h"
#include "motor_drive.h"
#include "nmea.h"
#include "uart_um980.h"

static const char *TAG = "ThingsBoard";
static esp_mqtt_client_handle_t client;

TaskHandle_t tb_gga_request_handler = NULL;

static bool tb_connected = false;

// ThingsBoard configuration
static char tb_mqtt_host[128];
static int tb_mqtt_port;
static char tb_mqtt_token[64];

static void mqtt_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

static int tb_send_rpc_response(const char *request_id, const char *response);
static void tb_rpc_callback(const char *method, cJSON *params, const char *request_id);

static void tb_telemetry_task(void *param);
static void tb_location_task(void *pvParameters);
static int tb_send_telemetry(const char *payload, bool retain);

void tb_init(const char *host, int port, const char *access_token)
{
    strncpy(tb_mqtt_host, host, sizeof(tb_mqtt_host) - 1);
    tb_mqtt_port = port;
    strncpy(tb_mqtt_token, access_token, sizeof(tb_mqtt_token) - 1);
}

esp_err_t tb_connect(void)
{
    char uri[150];
    snprintf(uri, sizeof(uri), "mqtt://%s:%d", tb_mqtt_host, tb_mqtt_port);

    esp_mqtt_client_config_t esp_mqtt_client_config = {
        .broker.address.uri = uri,
        .credentials.username = tb_mqtt_token, // Use access token as username for ThingsBoard
        // No password required for ThingsBoard when using access token
    };

    client = esp_mqtt_client_init(&esp_mqtt_client_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    xTaskCreate(tb_telemetry_task, 
                "tb_telemetry_task", 
                4096, 
                NULL, 
                5, 
                NULL);

    xTaskCreate(tb_location_task, 
                "tb_location_task", 
                4096, 
                NULL, 
                5, 
                NULL);

    return ESP_OK;
}


static void mqtt_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED to ThingsBoard");
        // Subscribe to RPC requests (for remote control)
        esp_mqtt_client_subscribe(client, "v1/devices/me/rpc/request/+", 1);
        tb_connected = true;
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        tb_connected = false;
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED");
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGD(TAG, "MQTT_EVENT_PUBLISHED");
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGD(TAG, "MQTT_EVENT_DATA");
        //printf("topic %.*s\n", event->topic_len, event->topic);

        // Handle RPC requests from ThingsBoard
        if (strncmp(event->topic, "v1/devices/me/rpc/request/", strlen("v1/devices/me/rpc/request/")) == 0)
        {
            // Process RPC commands here
            // printf("RPC request: %.*s\n", event->data_len, event->data);

            // Parse the JSON request
            cJSON *request_json = cJSON_Parse(event->data);
            if (request_json == NULL)
            {
                ESP_LOGE(TAG, "Failed to parse JSON");
                break;
            }

            cJSON *method = cJSON_GetObjectItem(request_json, "method");
            cJSON *params = cJSON_GetObjectItem(request_json, "params");
            // Extract request ID from topic (needed for response)
            char *request_id = event->topic + strlen("v1/devices/me/rpc/request/");

            // Call registered callback if any
            if (cJSON_IsString(method))
            {
                tb_rpc_callback(method->valuestring, params, request_id);
            }

            cJSON_Delete(request_json);
        }
        else
        {
            //printf("message: %.*s\n", event->data_len, event->data);
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "ERROR %s", strerror(event->error_handle->esp_transport_sock_errno));
        break;
    default:
        break;
    }
}

// TODO: refactor to SWITCH
static void tb_rpc_callback(const char *method, cJSON *params, const char *request_id)
{
    ESP_LOGD(TAG, "Received RPC method: %s", method);

    if (strcmp(method, "setState") == 0 && cJSON_IsBool(params))
    {
        // Set the LED state based on the params value
        bool led_state = cJSON_IsTrue(params);
        led_set_state(LED_BUILTIN, led_state);

        // Create simple JSON response with LED state
        char response[50];
        sprintf(response, "{\"value\":%s}", led_get_state(LED_BUILTIN) ? "true" : "false");
        // Send response back to ThingsBoard
        tb_send_rpc_response(request_id, response);
        ESP_LOGD(TAG, "Sent RPC response: %s", response);
    }
    else if (strcmp(method, "setGpioStatus") == 0 && cJSON_IsObject(params))
    {
        cJSON *pin = cJSON_GetObjectItem(params, "pin");
        cJSON *enabled = cJSON_GetObjectItem(params, "enabled");

        if (cJSON_IsNumber(pin) && cJSON_IsBool(enabled))
        {
            int gpio_pin = pin->valueint;
            bool gpio_state = cJSON_IsTrue(enabled);
            // Set the GPIO state based on the params values
            gpio_set_level(gpio_pin, gpio_state);

            // Create response payload
            cJSON *payload = cJSON_CreateArray();
            for (int i = 0; i < GPIO_PIN_COUNT; i++)
            {
                cJSON_AddItemToArray(payload, cJSON_CreateBool(gpio_get_level(i)));
            }

            char *response = cJSON_PrintUnformatted(payload);
            cJSON_Delete(payload);

            // Send response back to ThingsBoard
            tb_send_rpc_response(request_id, response);
            ESP_LOGD(TAG, "Sent RPC response: %s", response);

            free(response);
        }
    }
    else if (strcmp(method, "joystickControl") == 0 && cJSON_IsObject(params))
    {
        motor_drive_control_joystick(params);
    }
}

static int tb_send_rpc_response(const char *request_id, const char *response)
{
    char topic[100];
    snprintf(topic, sizeof(topic), "v1/devices/me/rpc/response/%s", request_id);
    return esp_mqtt_client_publish(client, topic, response, strlen(response), 1, false);
}

// For testing purposes
static void tb_telemetry_task(void *param)
{
    int count = 0;
    char *telemetry = NULL;

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(15000));

        // Create JSON payload for ThingsBoard
        // Format: {"temperature":25,"humidity":60,"count":123}
        cJSON *payload = cJSON_CreateObject();
        cJSON_AddNumberToObject(payload, "temperature", 25.0 + (float)(rand() % 10));
        cJSON_AddNumberToObject(payload, "humidity", 60.0 + (float)(rand() % 20));
        cJSON_AddNumberToObject(payload, "count", count++);
        cJSON_AddBoolToObject(payload, "valueLed", led_get_state(LED_BUILTIN));

        telemetry = cJSON_PrintUnformatted(payload);
        cJSON_Delete(payload);

        // Send to ThingsBoard telemetry topic
        ESP_LOGD(TAG, "Sending telemetry: %s", telemetry);
        
        if (!tb_connected)
        {
            free(telemetry);
            continue;
        }
        
        tb_send_telemetry(telemetry, false);
        free(telemetry);
    }
}

static void tb_location_task(void *pvParameters)
{
    char* location = NULL;
    gga_report_t gga_report;
    location_data_t location_data;

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));

        xTaskNotifyGive(tb_gga_request_handler);

        xQueueReceive(um980_gga_queue, &gga_report, portMAX_DELAY);

        ESP_LOGD(TAG, "GGA msg: %.*s", gga_report.size, gga_report.data);

        location_data = nmea_extract_location_data((const char*) gga_report.data);
        if (location_data.latitude == 0 && location_data.longitude == 0)
        {
            continue;
        }

        cJSON *payload = cJSON_CreateObject();
        cJSON_AddNumberToObject(payload, "latitude", location_data.latitude);
        cJSON_AddNumberToObject(payload, "longitude", location_data.longitude);

        location = cJSON_PrintUnformatted(payload);
        cJSON_Delete(payload);

        ESP_LOGD(TAG, "Sending location: %s", location);
        
        if (!tb_connected)
        {
            free(location);
            continue;
        }
        
        tb_send_telemetry(location, false);
        free(location);
    }
}

static int tb_send_telemetry(const char *payload, bool retain)
{
    return esp_mqtt_client_publish(client, "v1/devices/me/telemetry", payload, strlen(payload), 1, retain);
}
