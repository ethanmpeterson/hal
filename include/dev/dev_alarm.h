#ifndef DEV_ALARM_H
#define DEV_ALARM_H

#include <stdbool.h>
#include <stdint.h>

#include "hal.h"
#include "hal_rtc.h"
#include "dev_alarm_microSpecific.h"

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
  // 24 hour time
  uint8_t hour;
  uint8_t minute;

  // bitmask for what weekdays the alarm is enabled
  // Bit 0 represents Monday, bit 1 Tuesday etc. Bit 7 is unused
  uint8_t weekdayMask;
} dev_alarm_timeStamp_S;

// Each channel represents an alarm, each channel will have its own state but
// they will share a single hal_rtc reference time
typedef struct {
  dev_alarm_timeStamp_S alarmTime;
  // Optional callback that is called once on alarm activation
  void (*callback)(void);
} dev_alarm_channelConfig_S;

typedef struct {
  dev_alarm_channelConfig_S const * const channels;
} dev_alarm_config_S;

hal_error_E dev_alarm_init(dev_alarm_config_S const *const config);

dev_alarm_status_E dev_alarm_getStatus(dev_alarm_channel_E channel);
hal_error_E dev_alarm_setStatus(dev_alarm_channel_E channel, dev_alarm_status_E status);

// Called in a loop to run and update the state machine
void dev_alarm_runLoop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DEV_ALARM_H */
