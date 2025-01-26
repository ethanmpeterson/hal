// Standard Library Imports
#include <string.h>

// IO Layer Imports
#include "hal.h"
#include "hal_uart.h"

// Device Layer Imports
#include "dev_console.h"
#include "hal_uart_microSpecific.h"

typedef struct {
  // Both marked as volatile as they will modified in interrupt context
  char commandBuffer[DEV_CONSOLE_MAX_COMMAND_LENGTH];

  // tokenized version of the command buffer above
  char commandTokens[DEV_CONSOLE_MAX_COMMAND_ARGS][DEV_CONSOLE_MAX_ARG_LENGTH];

  dev_console_config_S const * config;
} dev_console_data_S;

static dev_console_data_S dev_console_data;
static dev_console_data_S * const data = &dev_console_data;

static hal_error_E dev_console_private_parseCommand(uint32_t *tokenCount);
static void dev_console_private_clearBuffers(void);

hal_error_E dev_console_init(dev_console_config_S const *const config) {
  memset(data, 0U, sizeof(dev_console_data_S));
  dev_console_private_clearBuffers();
  if (config == NULL) {
    return HAL_ERROR_ERR;
  }

  data->config = config;

  return HAL_ERROR_OK;
}

hal_error_E dev_console_processCommandString(char *commandString) {
  // First check that the length of the string is less than our max command length
  // If there is already a command waiting, we cannot populate the buffer
  if (strlen(commandString) >= DEV_CONSOLE_MAX_COMMAND_LENGTH) {
    return HAL_ERROR_ERR;
  }

  hal_error_E ret = HAL_ERROR_OK;

  // Now clear the module's buffer and copy the string
  dev_console_private_clearBuffers();
  (void)strcpy((char *)data->commandBuffer, commandString);

  uint32_t tokenCount;
  hal_error_E parseError = dev_console_private_parseCommand(&tokenCount);
  if (parseError != HAL_ERROR_OK) {
    hal_uart_sendString(data->config->consolePort, "ERROR: Number of arguments exceeded\n");
    return HAL_ERROR_ERR;
  }

  // If we parsed successfully and a command is ready, find the callback and run the operation.
  uint32_t i;
  for (i = 0; i < data->config->commandCount; i++) {
    if (!strcmp(data->commandTokens[0], data->config->commands[i].prefix)) {
      // If the first token passed matches a command callback exit the loop and run the operation
      break;
    }
  }

  // Needs to be done to avoid cast alignment warning
  // This will cause an unbounded stack usage warning since we don't know the
  // size of token count. So instead we just always make our args array the max
  // possible length
  char *arg[DEV_CONSOLE_MAX_COMMAND_ARGS];
  for (uint32_t i = 1; i < tokenCount; i++) {
    arg[i - 1] = data->commandTokens[i];
  }

  if (data->config->commands[i].callback(arg, tokenCount - 1) == HAL_ERROR_OK) {
    hal_uart_sendString(data->config->consolePort, "PASS\n");
  } else {
    hal_uart_sendString(data->config->consolePort, "ERROR\n");
    ret = HAL_ERROR_ERR;
  }

  return ret;
}

hal_error_E dev_console_parseDecimalDigit(char *token, uint32_t *word) {
  // Input sanity
  if (word == NULL || token == NULL) {
    return HAL_ERROR_ERR;
  }

  hal_error_E ret = HAL_ERROR_OK;

  *word = 0;
  uint32_t numberLength = strlen(token);
  for (uint32_t i = 0; i < numberLength; i++) {
    char digit = token[i];

    // Check that we have a valid ASCII digit
    if (digit >= '0' && digit <= '9') {
      // Do stuff
      uint8_t digitVal = digit - '0';
      uint32_t digitPos = numberLength - 1 - i;
      if (digitPos == 0) {
        // Handle units digit case
        *word += digitVal;
      } else {
        *word += 10 * (numberLength - 1 - i) * digitVal;
      }
    } else {
      // Invalid digit, set error code and exit
      ret = HAL_ERROR_ERR;
      break;
    }
  }

  return ret;
}

static hal_error_E dev_console_private_parseCommand(uint32_t *tokenCount) {
  hal_error_E ret = HAL_ERROR_OK;

  // Tokenize the string and collect the data in commandTokens pointer
  char *savePointer;
  char *token = strtok_r((char *)data->commandBuffer, " ", &savePointer);
  uint32_t tokNum = 0;
  while (token != NULL && tokNum < DEV_CONSOLE_MAX_COMMAND_ARGS) {
    // Now validate the token and copy it into our module data
    if (strlen(token) >= DEV_CONSOLE_MAX_ARG_LENGTH) {
      // if the argument is too long, return an error exit the loop
      ret = HAL_ERROR_ERR;
      break;
    }

    // For each token, copy the string into token buffer
    (void)strcpy(data->commandTokens[tokNum], token);

    // Fetch the next token
    token = strtok_r(NULL, " ", &savePointer);
    tokNum++;
  }

  // save the tokenCount for the command runner
  *tokenCount = tokNum;

  // Once we finish the loop, we need to validate that the command ends in a
  // newline and remove it from the last token.
  uint32_t tokenLength = strlen(data->commandTokens[tokNum - 1]);
  if (data->commandTokens[tokNum - 1][tokenLength - 1] == '\n') {
    // Replace the newline with null termination
    data->commandTokens[tokNum - 1][tokenLength - 1] = '\0';
  } else {
    ret = HAL_ERROR_ERR;
  }

  return ret;
}

static void dev_console_private_clearBuffers(void) {
  memset((void *)data->commandBuffer, 0U, sizeof(data->commandBuffer));
  memset((void *)data->commandTokens, 0U, sizeof(data->commandTokens));
}
