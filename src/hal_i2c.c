#include <string.h>

#include "hal_i2c.h"
#include "hal.h"
#include <stdbool.h>

typedef struct {
    hal_i2c_config_S const * config;
    volatile uint8_t receiveData[HAL_I2C_CHANNEL_COUNT][0xFF];
    volatile uint8_t sendData[HAL_I2C_CHANNEL_COUNT][0xFF];
    volatile uint8_t dataIndex[HAL_I2C_CHANNEL_COUNT];
    volatile bool isSending[HAL_I2C_CHANNEL_COUNT];
    volatile bool isReceiving[HAL_I2C_CHANNEL_COUNT];
    volatile uint8_t* receiveAddress[HAL_I2C_CHANNEL_COUNT];
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
    return config_data->config->channels[channel].sendData(address, data, nBytes);
}

hal_error_E hal_i2c_receiveData(hal_i2c_channel_E channel, uint16_t address, uint8_t* data, uint8_t nBytes) {
    return config_data->config->channels[channel].sendData(address, data, nBytes);
}

hal_error_E hal_i2c_IRQHandler(hal_i2c_channel_E channel) {
    return config_data->config->channels[channel].IRQ_Handler();
}

hal_error_E hal_i2c_data_transfer_complete(hal_i2c_channel_E channel) {
    if (config_data->isSending[channel]) {

        //Reset data buffer
        memset(config_data->sendData[channel], 0, 0xFF);
        config_data->dataIndex[channel] = 0;
        config_data->isSending[channel] = false;
        config_data->config->channels[channel].endTransmission();
    }
    if (config_data->isReceiving[channel]) {
        //Place data in frame passed when I2C began receiving, can maybe remove and just place data in that frame as it comes in
        memcpy(config_data->receiveData, config_data->receiveAddress[channel], ++config_data->dataIndex[channel]);

        memset(config_data->receiveData[channel], 0, 0xFF);
        config_data->dataIndex[channel] = 0;
        config_data->isReceiving[channel] = false;
        config_data->config->channels[channel].endTransmission();

    }
}

hal_error_E hal_i2c_begin_send(hal_i2c_channel_E channel, uint16_t address, uint8_t nBytes) {
    config_data->isSending[channel] = true;
    config_data->config->channels[channel].beginSend(address, nBytes);   
}

hal_error_E hal_i2c_begin_receive(hal_i2c_channel_E channel, uint16_t address, uint8_t nBytes, uint8_t* returnAddress) {
    config_data->isReceiving[channel] = true;
    config_data->receiveAddress[channel] = returnAddress;
    config_data->config->channels[channel].beginReceive(address, nBytes);
}

hal_error_E hal_i2c_receive_next_byte(hal_i2c_channel_E channel) {
    config_data->config->channels[channel].receiveNextByte(&config_data->receiveData[channel][config_data->dataIndex[channel]]);
    config_data->dataIndex[channel]++;
}

hal_error_E hal_i2c_transmit_next_byte(hal_i2c_channel_E channel) {
    config_data->config->channels[channel].sendNextByte(config_data->sendData[channel][config_data->dataIndex[channel]]);
    config_data->dataIndex[channel]++;
}