#ifndef CONFIG_H
#define CONFIG_H

#ifndef IS_MCU_VERSION
#define IS_MCU_VERSION 1
#endif

#ifndef LED_MAX_BRIGHTNESS
#define LED_MAX_BRIGHTNESS 50
#endif

#ifndef CONFIG_FILE_PATH
#define CONFIG_FILE_PATH "/config.json"
#endif

#ifndef DISK_SIZE_MB
#define DISK_SIZE_MB 1
#endif

#ifndef MAX_NUM_LAYERS
#define MAX_NUM_LAYERS 16
#endif

#ifndef AM_MAX_LOOP
#define AM_MAX_LOOP 200
#endif

#ifndef HID_USB_CONNECTION_TIMEOUT
#define HID_USB_CONNECTION_TIMEOUT 1000
#endif

#endif //CONFIG_H