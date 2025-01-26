#ifndef HAL_RTC_H
#define HAL_RTC_H

#include "hal.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t year; // interpreted as 20XX
  uint8_t month;
  uint8_t day;
  // 1 == Monday, 2 == Tuesday etc.
  uint8_t weekday;
  uint8_t hour;
  uint8_t minute;
  uint8_t seconds;
} hal_rtc_time_S;

typedef struct {
  hal_error_E (*initRealTimeClock)(void);
  hal_error_E (*setTimestamp)(const hal_rtc_time_S *time);
  hal_error_E (*getTimestamp)(hal_rtc_time_S *time);
} hal_rtc_config_S;


hal_error_E hal_rtc_init(hal_rtc_config_S const *const config);
hal_error_E hal_rtc_setTime(hal_rtc_time_S *time);
hal_error_E hal_rtc_getTime(hal_rtc_time_S *time);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HAL_RTC_H */
