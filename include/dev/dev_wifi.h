#ifndef DEV_WIFI_H
#define DEV_WIFI_H

#include <stdbool.h>
#include <stdint.h>

#include "hal.h"
#include "hal_uart.h"
#include "hal_uart_microSpecific.h"

#define DEV_WIFI_MAX_COMMAND_LENGTH 7
#define DEV_WIFI_MAX_COMMAND_ARGS   6
#define DEV_WIFI_MAX_ARG_LENGTH     1
#define DEV_WIFI_COMMAND_END        4

#ifdef __cplusplus
extern "C" {
#endif

//This stores all the command types. 
//Since the commands will be sent from the ESP, there is no need for the command identifier to be human readable
typedef enum {

    DEV_WIFI_COMMAND_SET_TIME,
    DEV_WIFI_COMMAND_GET_TIME,
    DEV_WIFI_COMMAND_SET_ALARM,
    DEV_WIFI_COMMAND_GET_ALARMS,
    DEV_WIFI_COMMAND_REMOVE_ALARM

} dev_wifi_command_id_E;


typedef struct {

    dev_wifi_command_id_E id;
    
    // function pointer to the code that processes the command,
    // the callback is called with an array of arguments and the number of args.
    // The array of arguments includes everything including the ID.
    hal_error_E (*callback)(uint8_t *args, uint8_t argsLen);
} dev_wifi_command_S;

typedef struct {
    // The UART channel used to collect and parse command strings
    const hal_uart_channel_E wifiPort;

    // Lookup table of the command callbacks to parse strings
    dev_wifi_command_S const * const commands;
    const uint8_t commandCount;
} dev_wifi_config_S;

hal_error_E dev_wifi_init(dev_wifi_config_S const *const config);
hal_error_E dev_wifi_processCommandArray(uint8_t *commandArray, uint8_t arrLen);

#ifdef __cplusplus
}
#endif

#endif