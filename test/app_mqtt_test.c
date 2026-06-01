#include "app_mqtt.h"

int func(char *data, int data_len);

int main(int argc, char const *argv[])
{
    app_mqtt_init();
    // 客户端注册处理函数
    app_mqtt_register_callback(&func);

    app_mqtt_sendMsg("hello world", strlen("hello world"));

    sleep(5);

    app_mqtt_destroy();
    return 0;
}

int func(char *data, int data_len)
{
    log_info("客户端注册处理数据\n");
    log_info("data:%s,len:%d\n", data, data_len);
    return 0;
}
