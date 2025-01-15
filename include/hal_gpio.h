#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "hal.h"
#include "hal_gpio_microSpecific.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  HAL_GPIO_PINMODE_OUTPUT_PUSH_PULL,
  HAL_GPIO_PINMODE_OUTPUT_OPEN_DRAIN,
  HAL_GPIO_PINMODE_INPUT,

  HAL_GPIO_PINMODE_COUNT,

  HAL_GPIO_PINMODE_UNKNOWN,
} hal_gpio_pinMode_E;

typedef enum {
  HAL_GPIO_PULLMODE_NONE,
  HAL_GPIO_PULLMODE_PULLUP,
  HAL_GPIO_PULLMODE_PULLDOWN,

  HAL_GPIO_PULLMODE_COUNT,

  HAL_GPIO_PULLMODE_UNKNOWN,
} hal_gpio_pullMode_E;

typedef enum {
  HAL_GPIO_PINSTATE_ON,
  HAL_GPIO_PINSTATE_OFF,

  HAL_GPIO_PINSTATE_COUNT,

  HAL_GPIO_PINSTATE_UNKNOWN,
} hal_gpio_pinState_E;

typedef struct {
  hal_error_E (*initChannel)(void);
  hal_gpio_pinState_E (*readPinState)(void);
  hal_error_E (*setPinState)(hal_gpio_pinState_E pinState);
  hal_error_E (*setPinMode)(hal_gpio_pinMode_E pinMode);
  hal_error_E (*setPullMode)(hal_gpio_pullMode_E pullMode);
} hal_gpio_channelConfig_S;

typedef struct {
  hal_gpio_channelConfig_S const * const channels;
  const uint8_t channelCount;
} hal_gpio_config_S;

hal_error_E hal_gpio_init(hal_gpio_config_S const *const config);

hal_gpio_pinState_E hal_gpio_readInputState(hal_gpio_channel_E channel);
hal_error_E hal_gpio_setOutputState(hal_gpio_channel_E channel, hal_gpio_pinState_E pinState);

// Should not be needed during runtime in most cases but included anyways
hal_error_E hal_gpio_setPinMode(hal_gpio_channel_E channel);
hal_error_E hal_gpio_setPullMode(hal_gpio_channel_E channel);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HAL_GPIO_H */
