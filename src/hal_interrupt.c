#include <string.h>

#include "hal_interrupt.h"
#include "hal_interrupt_microSpecific.h"
#include "hal.h"

typedef struct {
    hal_interrupt_config_S const * config;
} hal_interrupt_data_S;

static hal_interrupt_data_S hal_interrupt_data;
static hal_interrupt_data_S * const data = &hal_interrupt_data;

hal_error_E hal_interrupt_init(hal_interrupt_config_S const *const config) {
    //zero out the data struct
    memset(data, 0U, sizeof(hal_interrupt_data_S));
    hal_error_E ret = HAL_ERROR_OK;
    // If the config is not NULL, assuming all other sub pointers have also been
    // assigned properly
    if (config != NULL) {
        data->config = config;
        for (uint8_t i = 0; i < config->channelCount; i++) {
            const hal_interrupt_channelConfig_S *const channel = &config->channels[i];
            hal_error_E channelInitError = channel->initChannel();

            if (channelInitError != HAL_ERROR_OK) {
                ret = HAL_ERROR_ERR;
            }
        }
    } else {
        // return an error if the config is NULL
        ret = HAL_ERROR_ERR;
    }

    return ret;
}