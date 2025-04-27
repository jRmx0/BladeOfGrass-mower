// TODO: Detect not responding um980

#include "config_board.h"

#include "nmea.h"
#include "uart_um980.h"

#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "wifi_monitor.h"

#define TAG "UM980"

#define UART_UM980 UART_NUM_1
#define UART_BAUD_RATE 115200

#define MESSAGE_INTERVAL_MS 1000 // obsolete

static QueueHandle_t um980_uart_queue;
#define UM980_UART_QUEUE_SIZE 1

QueueHandle_t um980_gga_queue;
#define UM980_GGA_QUEUE_SIZE 1

static void uart_um980_request_gga_task(void *pvParameters);
static void uart_um980_recieve_gga_task(void *pvParameters);

void uart_um980_init(void)
{
    um980_gga_queue = xQueueCreate(UM980_GGA_QUEUE_SIZE,
                                   sizeof(gga_report_t));

    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_ERROR_CHECK(uart_driver_install(UART_UM980,
                                        RX_BUFF_SIZE,
                                        TX_BUFF_SIZE,
                                        UM980_UART_QUEUE_SIZE,
                                        &um980_uart_queue,
                                        0));

    ESP_ERROR_CHECK(uart_param_config(UART_UM980,
                                      &uart_config));
                                      
    ESP_ERROR_CHECK(uart_set_pin(UART_UM980,
                                 UART_TX_PIN,
                                 UART_RX_PIN,
                                 UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE));

    xTaskCreate(uart_um980_request_gga_task,
                "um980_request_gga_task",
                4096,
                NULL,
                6,
                &wifi_monitor_um980_handler);

    xTaskCreate(uart_um980_recieve_gga_task,
                "uart_um980_recieve_gga_task",
                4096,
                NULL,
                6,
                NULL);
}

// TODO: 
// - Convert to requesting gga once
// - Replace task notification by event group:
//   - WIFI_CONNECTED (redundant)
//   - TB_CONNECTED (redundant)
//   - REQUEST_RECEIVED (might keep task notification)
static void uart_um980_request_gga_task(void *pvParameters)
{
    uint32_t message_count = 0;
    uint32_t notification_value;
    const char *gga_request = "LOG GNGGA ONCE\r\n";

    while (1)
    {
        // Wait for initial notification or remain blocked until WiFi connects
        if (message_count == 0)
        {
            xTaskNotifyWait(0, 0, &notification_value, portMAX_DELAY);
            if (notification_value != WIFI_MONITOR_CONNECTED)
            {
                // If we get notified but WiFi is not connected, continue waiting
                continue;
            }
        }

        message_count++;
        
        uart_write_bytes(UART_UM980, gga_request, strlen(gga_request));
        
        // Wait for the next interval or a notification
        if (xTaskNotifyWait(0, 0, &notification_value, pdMS_TO_TICKS(MESSAGE_INTERVAL_MS)))
        {
            if (notification_value != WIFI_MONITOR_CONNECTED)
            {
                message_count = 0;
            }
        }
    }
}

static void uart_um980_recieve_gga_task(void *pvParameters)
{
    uart_event_t uart_event;
    gga_report_t gga_received;

    while (1)
    {
        if (xQueueReceive(um980_uart_queue, &uart_event, portMAX_DELAY))
        {
            switch (uart_event.type)
            {
            case UART_DATA:
                // ESP_LOGI(TAG, "UART_DATA");
                uart_read_bytes(UART_UM980, gga_received.data, uart_event.size, pdMS_TO_TICKS(1000));
                gga_received.size = uart_event.size;

                // printf("%d bytes received: %.*s\n", uart_event.size, uart_event.size - 2, (const char*) gga_received.data);

                if (nmea_is_gga_location_report((const char *)gga_received.data))
                {
                    xQueueOverwrite(um980_gga_queue,
                                    &gga_received);
                }
                break;

            case UART_BUFFER_FULL:
                ESP_LOGW(TAG, "UART_BUFFER_FULL");
                uart_flush_input(UART_UM980);
                xQueueReset(um980_uart_queue);
                break;

            case UART_FIFO_OVF:
                ESP_LOGW(TAG, "UART_FIFO_OVF");
                uart_flush_input(UART_UM980);
                xQueueReset(um980_uart_queue);
                break;

            default:
                break;
            }
        }
    }
}

