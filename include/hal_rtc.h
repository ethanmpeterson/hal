#ifndef HAL_RTC_H
#define HAL_RTC_H

#include "hal.h"
/* #include "hal_rtc_microSpecific.h" */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
  hal_error_E (*initRealTimeClock)(void);
  // TODO: functions to fetch a timestamp and calendar fields
  // Functions to set an alarm with a timestamp or calendar time and also an alarm handler
} hal_rtc_config_S;

typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t seconds;
} hal_rtc_time_S;

hal_error_E hal_rtc_init(hal_rtc_config_S const *const config);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HAL_RTC_H */
