#ifndef SERVER_H
#define SERVER_H

#include "cJSON.h"
#include "esp_err.h"

#define WS_TYPE_UNSPECIFIED 0
#define WS_TYPE_ONOPEN      1
#define WS_TYPE_JOYSTICK    2

void init_server();
//void start_mdns_service();
void mount_fs();
int getTypeId(cJSON *type);
void log_heap_usage();

esp_err_t send_ws_message(char *message);

#endif
