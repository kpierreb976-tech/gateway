#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cJSON.h"
#include "log.h"

int main(int argc, char const *argv[])
{

    cJSON *root = cJSON_CreateObject();

    cJSON_AddItemToObject(root, "name", cJSON_CreateString("zhangsan"));
    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(18));
    cJSON_AddItemToObject(root, "is_male", cJSON_CreateBool(true));

    char *json_str = cJSON_PrintUnformatted(root);
    log_info("JSON String: %s", json_str); // 加个前缀，输出更清晰

    cJSON *json_parse = cJSON_Parse(json_str);

    // 防御性编程：在访问之前，先判断 json_parse 是否为空
    if (json_parse != NULL)
    {
        cJSON *name_json = cJSON_GetObjectItem(json_parse, "name");
        log_info("Name: %s", name_json->valuestring);

        cJSON *age_json = cJSON_GetObjectItem(json_parse, "age");
        log_info("Age: %d", age_json->valueint);

        cJSON *is_male_json = cJSON_GetObjectItem(json_parse, "is_male");
        log_info("Is Male: %d", is_male_json->valueint);
    }

    // 内存释放顺序：先释放字符串，再删除解析出的树，最后删除根对象
    free(json_str);
    cJSON_Delete(json_parse);
    cJSON_Delete(root);

    return 0;
}
