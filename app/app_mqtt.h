#pragma once

#include <MQTTClient.h>
#include <unistd.h>
#include <string.h>
#include "log.h"

#define ADDRESS "ws://192.168.184.1:1883"
#define CLIENTID "f9daa00d-3b84-4768-a714-1819a3df0c04"
#define TOPIC_PUSH "gateway_to_web1"
#define TOPIC_PULL "web_to_gateway1"
#define QOS 1
#define TIMEOUT 10000L

int app_mqtt_init(void);

int app_mqtt_sendMsg(char *data, int data_len);

void app_mqtt_destroy(void);

void app_mqtt_register_callback(int (*callback)(char *data, int data_len));
