// Standard Library Imports
#include <string.h>

// IO Layer Imports
#include "hal.h"
#include "hal_uart.h"

// Device Layer Imports
#include "dev_console.h"

typedef struct {
  char *commandBuffer[DEV_CONSOLE_MAX_COMMAND_LENGTH];
  dev_console_config_S const * config;
} dev_console_data_S;

static dev_console_data_S dev_console_data;
static dev_console_data_S * const data = &dev_console_data;

// NOTE: maybe consider a new enum for device specific error codes, ex. dev_console_error_E
hal_error_E dev_console_init(dev_console_config_S const *const config) {
  memset(data, 0U, sizeof(dev_console_data_S));
  memset(data->commandBuffer, 0U, DEV_CONSOLE_MAX_COMMAND_LENGTH * sizeof(char));

  if (config == NULL) {
    return HAL_ERROR_ERR;
  }

  data->config = config;

  return HAL_ERROR_OK;
}

