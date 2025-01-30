#ifndef DEV_WIFI_H
#define DEV_WIFI_H

#include <stdbool.h>
#include <stdint.h>

#include "hal.h"
#include "hal_uart.h"
#include "hal_uart_microSpecific.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

    const hal_uart_channel_E wifiPort;

} dev_wifi_config_S;

hal_error_E dev_wifi_init(dev_wifi_config_S const *const config);

#ifdef __cplusplus
}
#endif

#endif