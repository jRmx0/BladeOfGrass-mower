#ifndef SERVER_H
#define SERVER_H

#include "esp_log.h"
#include "esp_http_server.h"
#include "mdns.h"
#include "toggleLed.h"
#include "cJSON.h"
#include "pushBtn.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include <stdlib.h>

#define WS_TYPE_UNSPECIFIED 0
#define WS_TYPE_ONOPEN      1
#define WS_TYPE_JOYSTICK    2

void init_server();
void start_mdns_service();
void mount_fs();
int getTypeId(cJSON *type);

#endif
