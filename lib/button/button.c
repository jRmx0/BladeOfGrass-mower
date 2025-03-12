#include "button.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "cJSON.h"

#define TAG "BUTTON"

#define BUTTON_BUILTIN_GPIO GPIO_NUM_0

static gpio_num_t button_gpio[BUTTON_MAX];
static bool button_state[BUTTON_MAX];
static SemaphoreHandle_t button_sem[BUTTON_MAX];

static void IRAM_ATTR button_isr_handler(void *args)
{
    button_id_t button = (button_id_t) args;

    // Simple debouncing guard
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(button_sem[button], &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken)
        portYIELD_FROM_ISR();
}

static void button_task(void *params)
{
    button_id_t button = (button_id_t) params;

    while (true)
    {
        if (xSemaphoreTake(button_sem[button], portMAX_DELAY) == pdTRUE)
        {
            bool pressed = (gpio_get_level(button_gpio[button]) == 0); // Assuming active-low
            button_state[button] = pressed;

            cJSON *payload = cJSON_CreateObject();
            cJSON_AddBoolToObject(payload, "btn_state", pressed);
            char *message = cJSON_Print(payload);
            
            ESP_LOGI(TAG, "Button %d state: %s", button, pressed ? "PRESSED" : "RELEASED");
            // send_ws_message(message);

            cJSON_Delete(payload);
            free(message);
        }
    }
}

void button_init(button_id_t button, gpio_num_t gpio_num)
{
    if (button >= BUTTON_MAX)
    {
        ESP_LOGE(TAG, "Invalid button init: %d", button);
        return;
    }

    button_gpio[button] = gpio_num;
    button_state[button] = false;

    button_sem[button] = xSemaphoreCreateBinary();

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << gpio_num),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };

    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(gpio_num, button_isr_handler, (void *) button);

    xTaskCreate(button_task, "button_task", 2048, (void *) button, 5, NULL);
}

void buttons_init(void)
{
    button_init(BUTTON_BUILTIN, BUTTON_BUILTIN_GPIO);
}

bool button_is_pressed(button_id_t button)
{
    if (button >= BUTTON_MAX)
    {
        ESP_LOGE(TAG, "Invalid button is_pressed: %d", button);
        return false;
    }

    return (gpio_get_level(button_gpio[button]) == 0);
}

bool button_get_state(button_id_t button)
{
    if (button >= BUTTON_MAX)
    {
        ESP_LOGE(TAG, "Invalid button get_state: %d", button);
        return false;
    }

    return button_state[button];
}