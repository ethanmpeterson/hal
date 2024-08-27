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
    ret = HAL_ERROR_OK;
  }

  return ret;
}

