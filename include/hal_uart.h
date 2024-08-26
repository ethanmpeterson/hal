#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>

#include "hal.h"
#include "hal_uart_microSpecific.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct {
    // NOTE: More can be added to this interface to support DMA streaming or asynchronous transfers
    hal_error_E (*initChannel)(void);
    hal_error_E (*sendByte)(uint8_t data);
    hal_error_E (*receiveByte)(volatile uint8_t *data);
  } hal_uart_channelConfig_S;

  typedef struct {
    // constant address and constant data please
    hal_uart_channelConfig_S const * const channels;
    const uint8_t channelCount;
  } hal_uart_config_S;

  hal_error_E hal_uart_init(hal_uart_config_S const *const config);

  // Helper functions for interacting UART channels (I.e UART peripherals on the
  // micro)
  hal_error_E hal_uart_sendByte(hal_uart_channel_E channel, uint8_t byte);
  hal_error_E hal_uart_sendChar(hal_uart_channel_E channel, char c);

  // NOTE: NOT a string, just a single character receiver. Higher level logic can handle polling and construction of a string
  hal_error_E hal_uart_receiveChar(hal_uart_channel_E channel, char *c);
  hal_error_E hal_uart_receiveByte(hal_uart_channel_E channel, uint8_t *byte);

  // String and byte array functions
  hal_error_E hal_uart_sendString(hal_uart_channel_E channel, char *string);
  hal_error_E hal_uart_sendByteArray(hal_uart_channel_E channel, uint8_t *bytes, uint32_t length);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HAL_UART_H */
