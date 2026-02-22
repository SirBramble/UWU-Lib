#include "color.h"
#include <cstdint>

using namespace uwu;

inline color_t rainbow_next(color_t c, uint8_t pos_inc)
{
    int16_t r = c.r;
    int16_t g = c.g;
    int16_t b = c.b;

    // Red → Yellow
    if (r == 255 && g < 255 && b == 0)
    {
        g = g + pos_inc;
        if(g > 255)
        {
            r -= g-255;
            g = 255;
        }
    }
    // Yellow → Green
    else if (g == 255 && r > 0 && b == 0)
    {
        r -= pos_inc;
        if(r < 0)
        {
            b -= r; //b += -r
            r = 0;
        }

    }
    // Green → Cyan
    else if (g == 255 && b < 255 && r == 0)
    {
        b += pos_inc;
        if(b > 255)
        {
            g -= b-255;
            b = 255;
        }
    }
    // Cyan → Blue
    else if (b == 255 && g > 0 && r == 0)
    {
        g -= pos_inc;
        if(g < 0)
        {
            r -= g; //r += -g
            g = 0;
        }
    }
    // Blue → Magenta
    else if (b == 255 && r < 255 && g == 0)
    {
        r += pos_inc;
        if(r > 255)
        {
            b -= r-255;
            r = 255;
        }
    }
    // Magenta → Red
    else if (r == 255 && b > 0 && g == 0)
    {
        b -= pos_inc;
        if(b < 0)
        {
            r += b; //r -= -b
            b = 0;
        }
    }
    else
    {
        r = 255; g = 0; b = 0;
    }

    return color_t{ static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), c.a };
}


bool uwu::apply_layer_color_effect(layer_color_effect_t effect, uint8_t speed, color_t arg_color, color_t *key_colors, std::size_t size)
{
    if(key_colors == nullptr)
        return false;

    if(size == 0)
        return false;

    switch (effect)
    {
        case layer_color_effect_t::NONE:
            for(std::size_t i = 0; i < size; i++)
                if(key_colors[i].a == 0)
                    key_colors[i] = {0,0,0,0};
            break;
        case layer_color_effect_t::RAINBOW:
            for(int i = 0; i < size; i++)
                if(key_colors[i].a == 0)
                  key_colors[i] = rainbow_next(key_colors[0], (i+1) * speed);
            break;
        case layer_color_effect_t::CONST_COLOR:
            for(int i = 0; i < size; i++)
                if(key_colors[i].a == 0)
                  key_colors[i] = arg_color;
                break;
        default:
            break;
    }
    return true;
}
