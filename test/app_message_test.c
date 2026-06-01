#include "log.h"
#include "app_message.h"
#include "app_common.h"

int main(int argc, char **argv)
{
    char bytes[9] = {0x01, 0x02, 0x04, 'X', 'X', 'a', 'b', 'c', 'd'};
    char *json = app_message_bytes_to_json(bytes, sizeof(bytes));
    log_info("json: %s", json);

    char buffer_tmp[100];
    int bytes_len = app_message_json_to_bytes(json, buffer_tmp, sizeof(buffer_tmp));
    log_info("buffer_tmp: %s", buffer_tmp);

    char *new_json = app_message_bytes_to_json(buffer_tmp, bytes_len);
    log_info("new_json: %s", new_json);
    return 0;
}
