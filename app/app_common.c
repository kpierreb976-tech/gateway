#include "app_common.h"

long app_common_getCurrentTime(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

char *app_common_bytes_to_hexstr(char *bytes, int bytes_len)
{
    char *hexstr = (char *)malloc(bytes_len * 2 + 1);
    for (int i = 0; i < bytes_len; i++)
    {
        sprintf(hexstr + i * 2, "%02x", bytes[i]);
    }
    return hexstr;
}

int app_common_hexstr_to_bytes(char *hexstr, char *bytes)
{
    int hex_str_len = strlen(hexstr);
    for (int i = 0; i < hex_str_len; i += 2)
    {
        char hex_byte[3];
        hex_byte[0] = hexstr[i];
        hex_byte[1] = hexstr[i + 1];
        hex_byte[2] = '\0';
        bytes[i / 2] = strtol(hex_byte, NULL, 16);
    }
    return hex_str_len / 2;
}
