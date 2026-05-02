#include "rgbled.h"
#include "color.h"
#include "config.h"
#include <cstdint>

using namespace uwu;

#if IS_MCU_VERSION != 0

rgbled::rgbled(uint8_t pin, size_t led_count, uint8_t max_brightness) :
    m_pixels(led_count, pin, NEO_GRB + NEO_KHZ800),
    m_led_count(led_count),
    m_max_brightness(max_brightness)
{
    
}

void rgbled::init(color_t* key_colors)
{
    m_key_colors = key_colors;

    m_pixels.begin();
    m_pixels.clear();
    m_pixels.show();
}

void rgbled::send()
{
    if(m_key_colors == nullptr)
        return;

    m_pixels.clear();

    for (int i = 0; i < m_led_count; i++)
    {
        color_t c = m_key_colors[i];
        uint16_t r = (c.r*m_max_brightness)/255;
        uint16_t g = (c.g*m_max_brightness)/255;
        uint16_t b = (c.b*m_max_brightness)/255;
        m_pixels.setPixelColor(i, m_pixels.Color(r, g, b));
    }
    m_pixels.show();
}

#else
rgbled::rgbled(uint8_t pin, uint8_t led_count) :
    m_led_count(led_count)
{
}

void rgbled::init()
{
    return;
}

void rgbled::send()
{
    return;
}
#endif