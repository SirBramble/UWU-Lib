#ifndef MACRUWU_H
#define MACRUWU_H

#include "led_wall.h"
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

#define MACRUWU_NUM_KEYS 32


inline const uint8_t c_macruwu_key_to_pin_table[MACRUWU_NUM_KEYS] = 
{
    EGP0_7, EGP0_3, EGP1_7, EGP1_3, 32, 36, 40, 0,
    EGP0_6, EGP0_2, EGP1_6, EGP1_2, 33, 37, 41, 46,
    EGP0_5, EGP0_1, EGP1_5, EGP1_1, 34, 38, 42, 45,
    EGP0_4, EGP0_0, EGP1_4, EGP1_0, 35, 39, 43, 44
};

inline const int c_macruwu_key_to_led_table[MACRUWU_NUM_KEYS] =       // KEY -> LED_POS
{
    31, 30, 29, 28, 27, 26, 25, 24,
    23, 22, 21, 20, 19, 18, 17, 16,
    15, 14, 13, 12, 11, 10, 9, 8,
    7, 6, 5, 4, 3, 2, 1, 0
};

class macruwu : public module<MACRUWU_NUM_KEYS>
{
public:
    macruwu(const char* name, kts1622* expander, rgbled* rgbled, led_wall* led_wall) :
        module(name, c_macruwu_key_to_led_table, led_wall),
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