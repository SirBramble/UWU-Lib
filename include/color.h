#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

namespace uwu
{

enum class layer_color_effect_t : uint8_t
{
    NONE = 0,
    RAINBOW,
    CONST_COLOR
};

enum class key_color_effect_t : uint8_t
{
    NONE = 0, // Layer bound (no override)
    PRESSED,
    RELEASED,
    PRESSED_AND_RELEASED,
    CONST_COLOR
};

struct color_t
{
    uint8_t r=0, g=0, b=0, a=0;

    [[nodiscard]] constexpr uint32_t to_u32() const {
        return (uint32_t(r) <<  0) |
               (uint32_t(g) <<  8) |
               (uint32_t(b) << 16) |
               (uint32_t(a) << 24);
    }

    static constexpr color_t from_u32(uint32_t v) {
        return color_t{
            uint8_t((v >>  0) & 0xFF),
            uint8_t((v >>  8) & 0xFF),
            uint8_t((v >> 16) & 0xFF),
            uint8_t((v >> 24) & 0xFF)
        };
    }
};

bool apply_layer_color_effect(layer_color_effect_t effect, uint8_t speed, color_t arg_color, color_t* key_colors, std::size_t size);

}

#endif // COLOR_H
