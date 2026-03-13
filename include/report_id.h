#ifndef REPORT_ID_H
#define REPORT_ID_H

#include <cstdint>

#define NUM_REPORT_IDs 4    // one more as ID=0 is invalid

/**
 * @brief Report IDs for HID Interface
 * IMPOTRANT: No ID may be 0! RID=0 is reserved for single device descriptors. When using multiple 0 is invalid
 *
 */
enum REPORT_ID_enum : uint8_t
{
    RID_KEYBOARD = 1,
    RID_GAMEPAD = 2,
    RID_CONSUMER_CONTROL = 3 // Media, volume etc ..
};

#endif // REPORT_ID_H
