#ifndef THINGSBOARD_H
#define THINGSBOARD_H

#include <stdbool.h>
#include "cJSON.h"
#include "esp_err.h"

void tb_init(const char *host, int port, const char *access_token);

esp_err_t tb_connect(void);

#endif // THINGSBOARD_H
