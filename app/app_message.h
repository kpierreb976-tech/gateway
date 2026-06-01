#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cJSON.h"
#include "log.h"
#include "app_common.h"

/**
 * @brief bytes to json
 *  字节数组格式: 01 02 03 6E 6E 41 42 43
 *  json格式: {"conn_type":1, "id":6E6E,"msg":414243}
 * @param bytes
 * @param bytes_len
 * @return char*
 */
char *app_message_bytes_to_json(char *bytes, int bytes_len);

/**
 * @brief json to bytes
 *  json格式: {"conn_type":1, "id":6E6E,"msg":414243}
 *  字节数组格式: 01 02 03 6E 6E 41 42 43
 * @param json
 * @return int
 */
int app_message_json_to_bytes(char *json_str, char *bytes, int bytes_len);
