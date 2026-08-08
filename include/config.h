#ifndef CONFIG_H
#define CONFIG_H

#ifndef IS_MCU_VERSION
#define IS_MCU_VERSION 1
#endif

/**
 * @brief Size of dynamic keycode buffer used to allocate all keycodes used by the config
 * 
 */
#ifndef KEYCODE_MEM_SIZE
#define KEYCODE_MEM_SIZE 10000
#endif

/**
 * @brief Maximum brightness of each key LED
 * 
 */
#ifndef KEY_LED_MAX_BRIGHTNESS
#define KEY_LED_MAX_BRIGHTNESS 50
#endif

/**
 * @brief LED Wall geometry total LED count
 * 
 */
#ifndef WALL_AMOUNT_LEDS
#define WALL_AMOUNT_LEDS 462
#endif

/**
 * @brief LED Wall geometry height LED count
 * 
 */
#ifndef WALL_LED_HEIGHT_COUNT
#define WALL_LED_HEIGHT_COUNT 7
#endif

/**
 * @brief Maximum brightness of the led display
 * @note Should be max. 10-15 to limit current consumption and save retinas.
 * 
 */
#ifndef WALL_LED_MAX_BRIGHTNESS
#define WALL_LED_MAX_BRIGHTNESS 2
#endif

/**
 * @brief Size of the filesystem mounted via USB.
 * 
 */
#ifndef DISK_SIZE_MB
#define DISK_SIZE_MB 1
#endif

/**
 * @brief Size of the filesystem cache. Should me multiple of of 4kB.
 * 
 */
#ifndef DISK_CACHE_SIZE_KB
#define DISK_CACHE_SIZE_KB 128
#endif

/**
 * @brief Path to the config file on the internal filesystem.
 * 
 */
#ifndef CONFIG_FILE_PATH
#define CONFIG_FILE_PATH "/config.json"
#endif

/**
 * @brief Maximum number of layers that can be addressed by the config.
 * 
 */
#ifndef MAX_NUM_LAYERS
#define MAX_NUM_LAYERS 16
#endif

/**
 * @brief Maximum loop iterations for the parser. Safeguard against infinite parse loop.
 * 
 */
#ifndef AM_MAX_LOOP
#define AM_MAX_LOOP 200
#endif

/**
 * @brief Time the System has to register over USB.
 * @note Currently unused.
 * 
 */
#ifndef HID_USB_CONNECTION_TIMEOUT
#define HID_USB_CONNECTION_TIMEOUT 1000
#endif

/**
 * @brief Size of text buffer for rendered text on the led display.
 * 
 */
#ifndef LED_WALL_NODE_TEXT_BUFFER_LEN
#define LED_WALL_NODE_TEXT_BUFFER_LEN 15
#endif


/**
 * @brief Size of the text buffer for the path pointing to a target gif in the filesystem.
 * 
 */
#ifndef LED_WALL_NODE_GIF_PATH_BUFFER_LEN
#define LED_WALL_NODE_GIF_PATH_BUFFER_LEN 1024
#endif

/**
 * @brief Size of the buffer of render elements for the led display. Changes the amount of render steps that can be defined in the config file.
 * 
 */
#ifndef LED_WALL_NODE_RENDER_BUFFER_LEN
#define LED_WALL_NODE_RENDER_BUFFER_LEN 32
#endif

#endif //CONFIG_H