#include <string.h>

#include "hal.h"
#include "hal_uart.h"

#include "dev_wifi.h"
#include "hal_uart_microSpecific.h"

typedef struct {

    dev_wifi_config_S const * config;

} dev_wifi_data_S;