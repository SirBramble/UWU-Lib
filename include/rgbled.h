#ifndef RGBLED_H
#define RGBLED_H

#include "color.h"

#if IS_MCU_VERSION == 0
    #include <cstdint>
    #define Adafruit_NeoPixel void
#else
    #include <Arduino.h>
    #include <Adafruit_NeoPixel.h>
#endif

namespace uwu
{

class rgbled 
{
public:
    rgbled(uint8_t pin, size_t led_count, uint8_t max_brightness);

    void init(color_t* key_colors);
    void send();

private:
    size_t m_led_count;
    Adafruit_NeoPixel m_pixels;
    color_t* m_key_colors = nullptr;

    uint8_t m_max_brightness = 0;
};

}

#endif //RGBLED_H