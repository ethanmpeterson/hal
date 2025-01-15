#include <string.h>

#include "hal.h"
#include "hal_rtc.h"
#include "hal_rtc_microSpecific.h"

typedef struct {
  hal_rtc_config_S const * config;
} hal_rtc_data_S;

static hal_rtc_data_S hal_rtc_data;
static hal_rtc_data_S * const data = &hal_rtc_data;

hal_error_E hal_rtc_init(hal_rtc_config_S const *const config) {
  memset(data, 0U, sizeof(hal_rtc_data_S));

  hal_error_E ret = HAL_ERROR_ERR;
  if (config != NULL) {
    data->config = config;

    // Call init function if given
    if (data->config->initRealTimeClock != NULL) {
        ret = data->config->initRealTimeClock();
    }
  }

  return ret;
}

hal_error_E hal_rtc_setTime(hal_rtc_time_S *time) {
  hal_error_E ret = HAL_ERROR_OK;
  if (data->config->setTimestamp != NULL) {
    data->config->setTimestamp(time);
  } else {
    ret = HAL_ERROR_ERR;
  }

  return ret;
}

hal_error_E hal_rtc_getTime(hal_rtc_time_S *time) {
  hal_error_E ret = HAL_ERROR_OK;
  if (data->config->getTimestamp != NULL) {
    data->config->getTimestamp(time);
  } else {
    ret = HAL_ERROR_ERR;
  }

  return ret;
}

