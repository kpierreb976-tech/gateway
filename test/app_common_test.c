#include "log.h"
#include "app_common.h"

int main(int argc, char *argv[])
{

    long current_time = app_common_getCurrentTime();
    log_info("Current time: %ld", current_time);

    return 0;
}
