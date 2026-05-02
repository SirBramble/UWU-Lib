#include "color.h"
#include <cstdint>
#include <cstdio>

#if IS_MCU_VERSION != 0
#include <Arduino.h>
#endif

using namespace uwu;

inline color_t rainbow_next(color_t c, uint16_t pos_inc)
{
    int r = c.r;
    int g = c.g;
    int b = c.b;

    // Recover invalid input
    bool valid =
        (r == 255 && g >= 0 && g <= 255 && b == 0) ||
        (r >= 0 && r <= 255 && g == 255 && b == 0) ||
        (r == 0 && g == 255 && b >= 0 && b <= 255) ||
        (r == 0 && g >= 0 && g <= 255 && b == 255) ||
        (r >= 0 && r <= 255 && g == 0 && b == 255) ||
        (r == 255 && g == 0 && b >= 0 && b <= 255);

    if (!valid) {
        r = 255;
        g = 0;
        b = 0;
    }

    while (pos_inc > 0)
    {
        // Red -> Yellow
        if (r == 255 && b == 0 && g < 255) {
            int step = min<int>(pos_inc, 255 - g);
            g += step;
            pos_inc -= step;
        }
        // Yellow -> Green
        else if (g == 255 && b == 0 && r > 0) {
            int step = min<int>(pos_inc, r);
            r -= step;
            pos_inc -= step;
        }
        // Green -> Cyan
        else if (r == 0 && g == 255 && b < 255) {
            int step = min<int>(pos_inc, 255 - b);
            b += step;
            pos_inc -= step;
        }
        // Cyan -> Blue
        else if (r == 0 && b == 255 && g > 0) {
            int step = min<int>(pos_inc, g);
            g -= step;
            pos_inc -= step;
        }
        // Blue -> Magenta
        else if (g == 0 && b == 255 && r < 255) {
            int step = min<int>(pos_inc, 255 - r);
            r += step;
            pos_inc -= step;
        }
        // Magenta -> Red
        else if (r == 255 && g == 0 && b > 0) {
            int step = min<int>(pos_inc, b);
            b -= step;
            pos_inc -= step;
        }
        else {
            r = 255;
            g = 0;
            b = 0;
            break;
        }
    }

    return color_t{
        .r = static_cast<uint8_t>(r),
        .g = static_cast<uint8_t>(g),
        .b = static_cast<uint8_t>(b),
        .a = c.a
    };
}


bool color_functions::apply_layer_color_effect(layer_color_effect_t effect, uint8_t speed, color_t arg_color, color_t *key_colors, std::size_t size)
{
    if (key_colors == nullptr || size == 0)
        return false;

    switch (effect)
    {
        case layer_color_effect_t::NONE:
            for (std::size_t i = 0; i < size; i++)
            {
                if (key_colors[i].a == 0)
                    key_colors[i] = {0, 0, 0, 0};
            }
            break;

        case layer_color_effect_t::RAINBOW:
        {
            // advance time once per frame
            m_layer_rainbow_base = rainbow_next(m_layer_rainbow_base, speed);

            // fixed spatial spacing between LEDs
            constexpr uint16_t offset_per_led = 8;

            for (std::size_t i = 0; i < size; i++)
            {
                if (key_colors[i].a == 0)
                    key_colors[i] = rainbow_next(m_layer_rainbow_base, i * offset_per_led);
            }
            break;
        }

        case layer_color_effect_t::CONST_COLOR:
            for (std::size_t i = 0; i < size; i++)
            {
                if (key_colors[i].a == 0)
                    key_colors[i] = arg_color;
            }
            break;

        default:
            break;
    }

    return true;
}

bool color_functions::apply_display_color_effect(display_color_effect_t effect, uint8_t speed, color_t arg_color, color_t* key_colors, std::size_t size)
{
    if (key_colors == nullptr || size == 0)
        return false;

    switch (effect)
    {
        case display_color_effect_t::NONE:
            for (std::size_t i = 0; i < size; i++)
            {
                if (key_colors[i].a == 0)
                    key_colors[i] = {0, 0, 0, 0};
            }
            break;

        case display_color_effect_t::RAINBOW:
        {
            // advance time once per frame
            m_display_rainbow_base = rainbow_next(m_display_rainbow_base, speed);

            // fixed spatial spacing between LEDs
            constexpr uint16_t offset_per_led = 1;

            for (std::size_t i = 0; i < size; i++)
            {
                if (key_colors[i].a == 0)
                    key_colors[i] = rainbow_next(m_display_rainbow_base, i * offset_per_led);
            }
            break;
        }

        case display_color_effect_t::CONST_COLOR:
            for (std::size_t i = 0; i < size; i++)
            {
                if (key_colors[i].a == 0)
                    key_colors[i] = arg_color;
            }
            break;

        default:
            break;
    }

    return true;
}
