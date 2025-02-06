#include <string.h>

#include "hal.h"
#include "hal_uart.h"

#include "dev_wifi.h"
#include "hal_uart_microSpecific.h"

#include "hal_rtc.h"

typedef struct {

    char commandBuffer[DEV_WIFI_MAX_COMMAND_LENGTH];

    char commandTokens[DEV_WIFI_MAX_COMMAND_ARGS][DEV_WIFI_MAX_ARG_LENGTH];

    dev_wifi_config_S const * config;

} dev_wifi_data_S;

static dev_wifi_data_S dev_wifi_data;
static dev_wifi_data_S * const data = &dev_wifi_data;

hal_error_E dev_wifi_init(dev_wifi_config_S const *const config) {
    memset(data, 0U, sizeof(dev_wifi_data_S));
    
    if (config == NULL) {
        return HAL_ERROR_ERR;
    }

    data->config = config;

    return HAL_ERROR_OK;
}

// Array should start with a unique beginning and ending byte. This is to ensure it is receiving a full command
hal_error_E dev_wifi_processCommandArray(uint8_t *commandArray) {

    switch(commandArray[0]) {
        case DEV_WIFI_COMMAND_SET_TIME:
            dev_wifi_setTime(commandArray);
            break;

        case DEV_WIFI_COMMAND_GET_TIME:
            dev_wifi_getTime();
            break;
        
        case DEV_WIFI_COMMAND_GET_ALARMS:
            dev_wifi_getAlarms();
            break;

        case DEV_WIFI_COMMAND_REMOVE_ALARM:
            dev_wifi_removeAlarm(commandArray);
            break;

        case DEV_WIFI_COMMAND_SET_ALARM:
            dev_wifi_setAlarm(commandArray);
            break;
        
        default:
            return HAL_ERROR_ERR;
            break;
    }
}

// Set time sent from wifi board
static hal_error_E dev_wifi_setTime(uint8_t *commandArray) {

    //Create the structure to pass to the hal_rtc function to set the time
    hal_rtc_time_S timeToSet = {
        .year = commandArray[1],
        .month = commandArray[2],
        .day = commandArray[3],

        .weekday = commandArray[4],
        .hour = commandArray[5],
        .minute = commandArray[6],
        .seconds = commandArray[7]
    };

    hal_error_E ret = hal_rtc_setTime(&timeToSet);

    return ret;
}

//Fill in the rest of these functions on each of their own branches
static hal_error_E dev_wifi_getTime(void) {

}

static hal_error_E dev_wifi_getAlarms(void) {

}

static hal_error_E dev_wifi_removeAlarm(uint8_t *commandArray) {

}

static hal_error_E dev_wifi_setAlarm(uint8_t *commandArray) {

}