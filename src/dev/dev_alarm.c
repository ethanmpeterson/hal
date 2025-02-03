// Standard Library Imports
#include <string.h>

// IO Layer Imports
#include "hal.h"
#include "hal_rtc.h"

// Device Layer Imports
#include "dev_alarm.h"
#include "dev_alarm_microSpecific.h"
#include "hal_uart.h"
#include "hal_uart_microSpecific.h"

typedef struct {
  dev_alarm_config_S const *config;

  // Stores the ACTIVE/INACTIVE statuses of each alarm
  dev_alarm_status_E channelStatuses[DEV_ALARM_CHANNEL_COUNT];

  // Stores the alarm time of each channel, changeable in the system console
  dev_alarm_timeStamp_S channelTimes[DEV_ALARM_CHANNEL_COUNT];
} dev_alarm_data_S;

static dev_alarm_data_S dev_alarm_data;
static dev_alarm_data_S * const data = &dev_alarm_data;

static dev_alarm_status_E dev_alarm_private_getDesiredState(dev_alarm_channel_E channel);

// Returns true if the alarm time matches the current RTC time, we only compare
// to minute precision.
static bool dev_alarm_private_isTimeStampEqual(dev_alarm_timeStamp_S alarmTime);

hal_error_E dev_alarm_init(dev_alarm_config_S const *const config) {
  memset(data, 0U, sizeof(dev_alarm_data_S));
  memset(data->channelStatuses, DEV_ALARM_STATUS_INACTIVE, sizeof(data->channelStatuses));
  memset(data->channelTimes, DEV_ALARM_STATUS_INACTIVE, sizeof(data->channelTimes));

  if (config == NULL) {
    return HAL_ERROR_ERR;
  }

  // Copy over the alarm times into modifiable data structure
  for (uint32_t i = 0; i < DEV_ALARM_CHANNEL_COUNT; i++) {
    data->channelTimes[i] = config->channels[i].alarmTime;
  }

  data->config = config;

  return HAL_ERROR_OK;
}

dev_alarm_status_E dev_alarm_getStatus(dev_alarm_channel_E channel) {
  dev_alarm_status_E ret = DEV_ALARM_STATUS_INACTIVE;
  if (channel < DEV_ALARM_CHANNEL_COUNT) {
    ret = data->channelStatuses[channel];
  }

  return ret;
}

hal_error_E dev_alarm_getAlarmTimeStamp(dev_alarm_channel_E channel, dev_alarm_timeStamp_S *timeStamp) {
  if (channel >= DEV_ALARM_CHANNEL_COUNT || timeStamp == NULL) {
    return HAL_ERROR_ERR;
  }

  *timeStamp = data->channelTimes[channel];

  return HAL_ERROR_OK;
}

hal_error_E dev_alarm_setAlarmTimeStamp(dev_alarm_channel_E channel, dev_alarm_timeStamp_S *timeStamp) {
  if (channel >= DEV_ALARM_CHANNEL_COUNT || timeStamp == NULL) {
    return HAL_ERROR_ERR;
  }

  data->channelTimes[channel] = *timeStamp;

  return HAL_ERROR_OK;
}

hal_error_E dev_alarm_setStatus(dev_alarm_channel_E channel, dev_alarm_status_E status) {
  hal_error_E ret = HAL_ERROR_ERR;
  if (channel < DEV_ALARM_CHANNEL_COUNT) {
    data->channelStatuses[channel] = status;
    ret = HAL_ERROR_OK;
  }

  return ret;
}

void dev_alarm_runLoop() {
  // poll all the alarms and update their status accordingly
  for (dev_alarm_channel_E channel = 0; channel < DEV_ALARM_CHANNEL_COUNT; channel++) {
    dev_alarm_status_E desiredState = dev_alarm_private_getDesiredState(channel);

    // if we have a transition from inactive to active, run the callback (if available)
    if (data->channelStatuses[channel] == DEV_ALARM_STATUS_INACTIVE &&
        desiredState == DEV_ALARM_STATUS_ACTIVE &&
        data->config->channels[channel].callback != NULL) {
      data->config->channels[channel].callback();
    }

    data->channelStatuses[channel] = desiredState;
  }
}

static dev_alarm_status_E dev_alarm_private_getDesiredState(dev_alarm_channel_E channel) {
  dev_alarm_status_E currentStatus = data->channelStatuses[channel];
  dev_alarm_status_E ret;

  switch (currentStatus) {
    case DEV_ALARM_STATUS_INACTIVE:
      // Check the time and update the status accordingly
      ret = (dev_alarm_private_isTimeStampEqual(data->config->channels[channel].alarmTime)) ? DEV_ALARM_STATUS_ACTIVE : DEV_ALARM_STATUS_INACTIVE;
      break;

    // If the alarm is active it needs to be dismissed by the application, also
    // return the current state if an invalid channel was passed
    case DEV_ALARM_STATUS_ACTIVE:
    case DEV_ALARM_STATUS_COUNT:
    case DEV_ALARM_STATUS_UNKNOWN:
    default:
        ret = currentStatus;
      break;
  }

  return ret;
}

static bool dev_alarm_private_isTimeStampEqual(dev_alarm_timeStamp_S alarmTime) {
  hal_rtc_time_S currentTime;
  hal_rtc_getTime(&currentTime);

  uint8_t weekdayMask = 1 << (currentTime.weekday - 1);

  return (currentTime.hour == alarmTime.hour &&
          currentTime.minute == alarmTime.minute &&
          (weekdayMask & alarmTime.weekdayMask));
}
