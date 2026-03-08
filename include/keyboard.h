#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "module.h"
#include "kts1622.h"
#include "rgbled.h"
#include "config.h"

#if IS_MCU_VERSION == 0
    #include <cstdint>
#else
    #include <Arduino.h>
#endif

namespace uwu
{


inline const uint8_t c_keyboard_row_pins[KEYBAORD_NUM_ROWS] = {2,3,4,5,6,7};

inline const uint8_t c_keyboard_col_pins[KEYBAORD_NUM_COLS] = {1,0, EGP1_0, EGP1_1, EGP1_2, EGP1_3, EGP1_4, EGP1_5, EGP1_6, EGP1_7, EGP0_0, EGP0_1, EGP0_2, EGP0_3, EGP0_4, EGP0_5, EGP0_6, EGP0_7};

inline const int c_keyboard_key_remap_table[KEYBAORD_NUM_ROWS][KEYBAORD_NUM_COLS] =
{
    {-1,  0, -1,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
    {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33},
    {34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51},
    {51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, -1, -1, -1},
    {66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, -1, -1, 80, -1},
    {81, 82, 83, 84, -1, -1, -1, 85, -1, -1, 86, 87, 88, 89, -1, 90, 91, 92}
};

inline const int c_keyboard_led_remap_table[KEYBOARD_NUM_KEYS] =       // KEY -> LED_POS
{
              92,           91,  90,  89,  88,  87, 86, 85, 84, 83, 82,    81, 80, 79, 78, 77,
    76, 75, 74, 73, 72, 71, 70, 69, 68,     67,     66,     65,     64,    63, 62, 61, 60, 59,
    58, 57, 56, 55, 54, 53, 52, 51, 50,     49,     48,     47,     46,    45, 44, 43, 42, 41,
    40, 39, 38, 37, 36, 35, 34, 33, 32,     31,     30,     29,     28,    27,
    26, 25, 24, 23, 22, 21, 20, 19, 18,     17,     16,     15,     14,    13,         12,
    11, 10,  9,  8,                             7,                 6, 5, 4, 3,      2,  1,  0
};

class keyboard : public module<KEYBOARD_NUM_KEYS>
{
public:
    keyboard(const char* name, kts1622* expander, rgbled* rgbled) :
        module(name, c_keyboard_led_remap_table),
        m_expander(expander),
        m_rgbled(rgbled)
        {}
    bool init();
    void update();
private:
    void pin_mode(uint8_t pin, PinMode mode);
    void digital_write(uint8_t pin, PinStatus state);
    bool digital_read(uint8_t pin);

    kts1622* m_expander = nullptr;
    rgbled* m_rgbled = nullptr;

};

}

#endif
