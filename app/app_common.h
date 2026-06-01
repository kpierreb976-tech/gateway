#pragma once

#include <sys/time.h>
#include <stdlib.h>
#include <log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long app_common_getCurrentTime(void);

/**
 * @brief bytes转16进制字符串
 * @param bytes
 * @param bytes_len
 * @return char*
 */
char *app_common_bytes_to_hexstr(char *bytes, int bytes_len);


int app_common_hexstr_to_bytes(char *hexstr, char *bytes);
