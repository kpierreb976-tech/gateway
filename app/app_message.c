#include "app_message.h"

char *app_message_bytes_to_json(char *bytes, int bytes_len)
{
    if (!bytes || bytes_len < 3)
    {
        return NULL;
    }

    uint8_t type = (uint8_t)bytes[0];
    uint8_t id_len = (uint8_t)bytes[1];
    uint8_t msg_len = (uint8_t)bytes[2];

    if (bytes_len < 3 + id_len + msg_len)
    {
        return NULL;
    }

    char *id_hexstr = app_common_bytes_to_hexstr(bytes + 3, id_len);
    if (!id_hexstr)
    {
        return NULL;
    }

    char *msg_hexstr = app_common_bytes_to_hexstr(bytes + 3 + id_len, msg_len);
    if (!msg_hexstr)
    {
        free(id_hexstr);
        return NULL;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root)
    {
        free(id_hexstr);
        free(msg_hexstr);
        return NULL;
    }

    cJSON_AddNumberToObject(root, "type", type);
    cJSON_AddStringToObject(root, "id", id_hexstr);
    cJSON_AddStringToObject(root, "msg", msg_hexstr);

    char *json_str = cJSON_PrintUnformatted(root);

    cJSON_Delete(root);
    free(id_hexstr);
    free(msg_hexstr);

    return json_str;
}

int app_message_json_to_bytes(char *json_str, char *bytes, int bytes_len)
{
    if (!json_str || !bytes || bytes_len < 3)
    {
        return -1;
    }

    cJSON *root = cJSON_Parse(json_str);
    if (!root)
    {
        log_error("json parse error\n");
        return -1;
    }

    // 提取并校验各字段
    cJSON *type_json = cJSON_GetObjectItem(root, "type");
    cJSON *id_json = cJSON_GetObjectItem(root, "id");
    cJSON *msg_json = cJSON_GetObjectItem(root, "msg");

    if (!type_json || !id_json || !msg_json)
    {
        log_error("json field missing\n");
        cJSON_Delete(root);
        return -1;
    }

    // 校验类型
    if (!cJSON_IsNumber(type_json) ||
        !cJSON_IsString(id_json) ||
        !cJSON_IsString(msg_json))
    {
        log_error("json field type error\n");
        cJSON_Delete(root);
        return -1;
    }

    uint8_t type = (uint8_t)type_json->valueint;

    // 十六进制字符串长度 / 2 = 字节数
    int id_len = strlen(id_json->valuestring) / 2;
    int msg_len = strlen(msg_json->valuestring) / 2;

    // 校验缓冲区
    int total_len = 3 + id_len + msg_len;
    if (bytes_len < total_len)
    {
        log_error("buffer too small: need %d, got %d\n", total_len, bytes_len);
        cJSON_Delete(root);
        return -1;
    }

    // 直接写入目标缓冲区对应位置
    bytes[0] = (char)type;
    bytes[1] = (char)id_len;
    bytes[2] = (char)msg_len;

    int ret = 0;
    ret = app_common_hexstr_to_bytes(id_json->valuestring, bytes + 3);
    if (ret < 0)
    {
        log_error("id hexstr to bytes error\n");
        cJSON_Delete(root);
        return -1;
    }
    ret = app_common_hexstr_to_bytes(msg_json->valuestring, bytes + 3 + id_len);
    if (ret < 0)
    {
        log_error("msg hexstr to bytes error\n");
        cJSON_Delete(root);
        return -1;
    }

    cJSON_Delete(root);
    return total_len;
}
