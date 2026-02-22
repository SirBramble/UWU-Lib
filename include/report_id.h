#ifndef REPORT_ID_H
#define REPORT_ID_H

#include <cstdint>

#define NUM_REPORT_IDs 3

/**
 * @brief Report IDs for HID Interface
 *
 */
enum REPORT_ID_enum : uint8_t
{
    RID_GAMEPAD = 0,
    RID_KEYBOARD = 1,
    RID_CONSUMER_CONTROL = 2 // Media, volume etc ..
};

#endif // REPORT_ID_H
