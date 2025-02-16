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

void init_server();
void start_mdns_service();
void mount_fs();

#endif
