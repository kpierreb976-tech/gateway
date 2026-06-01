#include "app_mqtt.h"

static MQTTClient client;                                                          // MQTT 客户端句柄
static MQTTClient_message message = MQTTClient_message_initializer;                // 发布消息结构体
static MQTTClient_connectOptions connOpts = MQTTClient_connectOptions_initializer; // 连接参数

static int (*recv_callback)(char *, int) = NULL;

static void connLost(void *context, char *cause);

int msgArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message);

void delivery(void *context, MQTTClient_deliveryToken dt);
int app_mqtt_init(void)
{
    // 1.创建MQTT客户端
    int result = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    if (result != MQTTCLIENT_SUCCESS)
    {
        log_info("MQTTClient_create error\n");
        return -1;
    }
    else
    {
        log_info("MQTTClient_create success\n");
    }

    // 2.设置客户端回调
    result = MQTTClient_setCallbacks(client, NULL, &connLost, &msgArrived, &delivery);
    if (result != MQTTCLIENT_SUCCESS)
    {
        log_info("MQTTClient_connectionLost error\n");
    }
    else
    {
        log_info("MQTTClient_connectionLost success\n");
    }

    // 3.连接
    result = MQTTClient_connect(client, &connOpts);
    if (result != MQTTCLIENT_SUCCESS)
    {
        log_info("MQTTClient_connect failed\n");
        return -1;
    }
    else
    {
        log_info("MQTTClient_connect success\n");
    }

    // 4.订阅
    result = MQTTClient_subscribe(client, TOPIC_PULL, QOS);
    if (result != MQTTCLIENT_SUCCESS)
    {
        log_info("MQTTClient_subscribe error\n");
    }
    else
    {
        log_info("MQTTClient_subscribe success\n");
    }
    return 0;
}

int app_mqtt_sendMsg(char *data, int data_len)
{
    // 发布
    message.payload = data;
    message.payloadlen = data_len;
    message.qos = QOS;
    MQTTClient_publishMessage(client, TOPIC_PUSH, &message, NULL);

    return 0;
}

void app_mqtt_destroy(void)
{
    // 断开连接
    int result = MQTTClient_disconnect(client, TIMEOUT);
    if (result != MQTTCLIENT_SUCCESS)
    {
        log_info("MQTTClient_disconnect error\n");
    }
    else
    {
        log_info("MQTTClient_disconnect success\n");
    }

    // 销毁
    MQTTClient_destroy(&client);
}

void app_mqtt_register_callback(int (*callback)(char *data, int data_len))
{
    recv_callback = callback;
}

/**
 * @brief 消息到达处理函数
 *
 * @param context
 * @param message
 */
static void connLost(void *context, char *cause)
{
    log_info("Connection lost:%s\n", cause);
}

/**
 * @brief 消息处理函数
 *
 * @param context
 * @param message
 */
int msgArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    log_info("Message arrived\n");
    log_info("Topic: %s\n", topicName);
    log_info("Payload: %s\n", message->payload);

    int result = 0;
    if (recv_callback != NULL)
    {
        result = recv_callback(message->payload, message->payloadlen) == 0 ? 1 : 0;
    }
    // 释放消息
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return result;
}

void delivery(void *context, MQTTClient_deliveryToken dt)
{
    log_info("MQTTClient_deliveryComplete\n");
}
