#ifndef HAL_INTERRUPT_H
#define HAL_INTERRUPT_H

#include "hal.h"
#include "hal_interrupt_microSpecific.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    hal_error_E (*initChannel)(void);
} hal_interrupt_channelConfig_S;

typedef struct {
    hal_interrupt_channelConfig_S const * const channels;
    const uint8_t channelCount;
} hal_interrupt_config_S;

hal_error_E hal_interrupt_init(hal_interrupt_config_S const *const config);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HAL_INTERRUPT_H */