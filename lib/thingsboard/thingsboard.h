#ifndef THINGSBOARD_H
#define THINGSBOARD_H

#include <stdbool.h>
#include "cJSON.h"

// Initialize ThingsBoard MQTT client
void tb_init(const char *host, int port, const char *access_token);

// Connect to ThingsBoard
bool tb_connect(void);

// Send telemetry data to ThingsBoard
int tb_send_telemetry(const char *payload, bool retain);

// Handle RPC commands
typedef void (*tb_rpc_callback_t)(const char *method, cJSON *params, const char *request_id);

// Register callback for RPC commands
void tb_register_rpc_callback(tb_rpc_callback_t callback);

// Must be called periodically to process MQTT events
void tb_task(void *param);

// Send response to RPC request
int tb_send_rpc_response(const char *request_id, const char *response);

// Standard RPC command handler
void tb_default_rpc_handler(const char *method, cJSON *params, const char *request_id);

// Start telemetry task
void tb_start_telemetry_task(void);

// Telemetry task
void tb_telemetry_task(void *param);

#endif // THINGSBOARD_H
