#ifndef DEV_CONSOLE_H
#define DEV_CONSOLE_H

#include <stdbool.h>
#include <stdint.h>

#include "hal.h"
#include "hal_uart.h"
#include "hal_uart_microSpecific.h"

#define DEV_CONSOLE_MAX_COMMAND_LENGTH 50
#define DEV_CONSOLE_MAX_COMMAND_ARGS   5
#define DEV_CONSOLE_MAX_ARG_LENGTH     50

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  // The command prefix / name. ex. rtc, wifi, etc.
  char *prefix;

  // function pointer to the code that processes the command,
  // the callback is called with an array of arguments and the number of args.
  // The array of arguments includes everything after the prefix. For example if the prefix was "rtc" and you sen the command "rtc x y", the arg array would be ["x", "y"] and args would be 2.
  hal_error_E (*callback)(char **arg, uint32_t args);
} dev_console_command_S;

typedef struct {
  // The UART channel used to collect and parse command strings
  const hal_uart_channel_E consolePort;

  // Lookup table of the command callbacks to parse strings
  dev_console_command_S const * const commands;
  const uint8_t commandCount;
} dev_console_config_S;

hal_error_E dev_console_init(dev_console_config_S const *const config);

// Given the pre-made string, parse the command and call the callback. Might be
// used in cases where you want to use a command at runtime without manually
// typing it into the console
hal_error_E dev_console_processCommandString(char *commandString);

// Returns true if a complete command is available in the module's internal buffer
// New commands will not be collected or processed until this is cleared.
// Will not support any queuing functionality for now.
bool dev_console_isCommandReady(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DEV_CONSOLE_H */

