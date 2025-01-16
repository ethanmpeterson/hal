#include <string.h>

#include "hal_i2c.h"
#include "hal.h"

typedef struct {
    hal_i2c_config_S const * config;
} hal_i2c_config_data_S;

static hal_i2c_config_data_S hal_i2c_config_data;
static hal_i2c_config_data_S * const config_data = &hal_i2c_config_data;

hal_error_E hal_i2c_init(hal_i2c_config_S const *const config) {

    memset(config_data, 0U, sizeof(hal_i2c_config_data_S));
    hal_error_E ret = HAL_ERROR_OK;

    if (config != NULL) {
        config_data->config = config;
        for (uint8_t i = 0; i < config->channelCount; i++) {
            const hal_i2c_channelConfig_S *const channel = &config->channels[i];
            hal_error_E channelInitError = channel->initChannel();

            if (channelInitError != HAL_ERROR_OK) {
                ret = HAL_ERROR_ERR;
            }
        }
    } else {
        ret = HAL_ERROR_ERR;
    }

    return ret;
}

hal_error_E hal_i2c_sendData(hal_i2c_channel_E channel, uint16_t address, uint8_t* data, uint8_t nBytes) {
    return config_data->config->channels[channel].sendData(address, &data, nBytes);
}

hal_error_E hal_i2c_receiveData(hal_i2c_channel_E channel, uint16_t address, uint8_t* data, uint8_t nBytes) {
    return config_data->config->channels[channel].sendData(address, &data, nBytes);
}
