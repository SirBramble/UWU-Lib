#include "layer.h"
#include "color.h"

using namespace uwu;

void _layer::print()
{
#if IS_MCU_VERSION == 0
    printf("------------------------ LAYER ------------------------\n");
    printf("color:\n\tr: %d, g: %d, b: %d\n", m_color.r, m_color.g, m_color.b);
    printf("color_effect: ");
    switch (m_effect)
    {
        case layer_color_effect_t::NONE:
            printf("none (%d)\n", m_effect);
            break;
        case layer_color_effect_t::RAINBOW:
            printf("rainbow (%d)\n", m_effect);
            break;
        case layer_color_effect_t::CONST_COLOR:
            printf("const (%d)\n", m_effect);
            break;
        default:
            printf("ERROR (%d)\n", m_effect);
            break;
    }
    printf("color_effect_speed: %d\n", m_effect_speed);
    printf("-------------------------------------------------------\n");
#endif
}
