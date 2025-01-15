#ifndef DEV_ALARM_H
#define DEV_ALARM_H

#include <stdbool.h>
#include <stdint.h>

#include "hal_rtc.h"
/* #include "dev_alarm_microSpecific.h" */

#ifdef __cplusplus
extern "C" {
#endif

// Functions to set an alarm with a timestamp or calendar time and also an
// alarm handler (put in dev_alarm module) Will also be cool to have an SW
// alarm for an arbitrary number of channels with logic in the dev_alarm
// module.

// dev_alarm can also use hal_microspecific functions to utilize alarms within
// the HW if applicable

  typedef enum {
    DEV_ALARM_STATUS_ACTIVE,
    DEV_ALARM_STATUS_INACTIVE,

    DEV_ALARM_STATUS_COUNT,
    DEV_ALARM_STATUS_UNKNOWN,
  } dev_alarm_status_E;

  typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    // bitmask for what weekdays the alarm is enabled
    uint8_t weekdayMask;
  } dev_alarm_timeStamp_S;

  typedef struct {
    /* void (*initAlarmChannel)(void); */

  } dev_alarm_channelConfig_S;

  typedef struct {
  } dev_alarm_config_S;

  void dev_alarm_init(dev_alarm_config_S const *const config);

  /* void dev_alarm_getAlarmStatus(dev_alarm_channel_E channel); */

  // TODO:
  // Set alarm active function (can be used in ISRs for HW alarms)
  // clear alarm active function (used in application to dismiss the alarm when applicable)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DEV_ALARM_H */
