#include <string.h>

#include "hal.h"
#include "hal_uart.h"

#include "dev_wifi.h"
#include "hal_uart_microSpecific.h"

#include "hal_rtc.h"

typedef struct {

    uint8_t commandBuffer[DEV_WIFI_MAX_COMMAND_LENGTH];

    dev_wifi_config_S const * config;

} dev_wifi_data_S;

static dev_wifi_data_S dev_wifi_data;
static dev_wifi_data_S * const data = &dev_wifi_data;

static void dev_wifi_private_clearBuffers(void);

hal_error_E dev_wifi_init(dev_wifi_config_S const *const config) {
    memset(data, 0U, sizeof(dev_wifi_data_S));
    
    if (config == NULL) {
        return HAL_ERROR_ERR;
    }

    data->config = config;

    return HAL_ERROR_OK;
}

// Need to adjust to ensure data is being passed properly
hal_error_E dev_wifi_processCommandArray(uint8_t *commandArray, uint8_t arrLen) {

    hal_error_E ret;

    if (arrLen >= DEV_WIFI_MAX_COMMAND_LENGTH) {
        return HAL_ERROR_ERR;
    }
    dev_wifi_private_clearBuffers();
    (void)memcpy((uint8_t *)data->commandBuffer, commandArray, arrLen);

    uint8_t i;
    for (i = 0; i < data->config->commandCount; i++) {
        if(data->config->commands[i].id == data->commandBuffer[0]) {
            break;
        }
    }

    if (data->config->commands[i].callback(data->commandBuffer, arrLen) == HAL_ERROR_OK) {
        // Send OK back to ESP
    } else {
        // Send ERR back to ESP
        ret = HAL_ERROR_ERR;
    }
    return ret;

}

static void dev_wifi_private_clearBuffers(void) {
    memset((void *)data->commandBuffer, 0U, sizeof(data->commandBuffer));
}