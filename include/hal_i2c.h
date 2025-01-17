#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stdint.h>
#include "hal.h"
#include "hal_i2c_microSpecific.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        hal_error_E (*initChannel)(void);
        hal_error_E (*sendData)(uint16_t address, uint8_t* data, uint8_t nBytes);
        hal_error_E (*receiveData)(uint16_t address, uint8_t* data, uint8_t nBytes);
        hal_error_E (*IRQ_Handler)(void);
        hal_error_E (*receiveNextByte)(uint8_t *receivedByte);
        hal_error_E (*sendNextByte)(uint8_t byteToSend);
        hal_error_E (*beginSend)(uint16_t address, uint8_t nBytes);
        hal_error_E (*beginReceive)(uint16_t address, uint8_t nBytes);
        hal_error_E (*endTransmission)(void);

    } hal_i2c_channelConfig_S;

    typedef struct {
        hal_i2c_channelConfig_S const * const channels;
        const uint8_t channelCount;
    } hal_i2c_config_S;


    hal_error_E hal_i2c_init(hal_i2c_config_S const * const config);
    hal_error_E hal_i2c_sendData(hal_i2c_channel_E channel, uint16_t address, uint8_t* data, uint8_t nBytes);
    hal_error_E hal_i2c_receiveData(hal_i2c_channel_E channel, uint16_t address, uint8_t* data, uint8_t nBytes);
    hal_error_E hal_i2c_IRQHandler(hal_i2c_channel_E);

    
    typedef enum {
        HAL_I2C_ADDRESS_MODE_7_BIT,
        HAL_I2C_ADDRESS_MODE_10_BIT
    } hal_i2c_address_mode_E;

    typedef enum {
        HAL_I2C_SPEED_MODE_STANDARD,
        HAL_I2C_SPEED_MODE_FAST,
        HAL_I2C_SPEED_MODE_FAST_PLUS
    } hal_i2c_speed_mode_E;

    typedef enum {
        HAL_I2C_MODE_MASTER_TRANSMITTER,
        HAL_I2C_MODE_MASTER_RECEIVER,
        HAL_I2C_MODE_SLAVE_TRANSMITTER,
        HAL_I2C_MODE_SLAVE_RECEIVER
    } hal_i2c_mode_E;

    typedef enum {
        HAL_I2C_TRANSFER,
        HAL_I2C_RECEIVE,
        HAL_I2C_NOOP
    } hal_i2c_transfer_type_E;

#ifdef __cplusplus
}
#endif
#endif