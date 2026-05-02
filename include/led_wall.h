#ifndef LED_WALL_H
#define LED_WALL_H

#include <Adafruit_GFX.h>
#include "color.h"
#include "rgbled.h"
#include "config.h"

namespace uwu
{

struct led_wall_render_node
{
    char text[LED_WALL_NODE_TEXT_BUFFER_LEN] = {'\0'};
    display_color_effect_t color_effect = display_color_effect_t::NONE;
    color_t color = {0,0,0,0};
    uint8_t color_effect_speed = 1;
    int64_t duration = 0;
};

class led_wall : public rgbled, public Adafruit_GFX, color_functions
{
public:
    led_wall(uint8_t pin);

    void init();
    void update();
    void clear();

    void clear_render_buffer();
    void render_update();

    void set_pixel(uint16_t x, uint16_t y, color_t color);

    // Adafruit_GFX override
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;

    led_wall_render_node* node(int pos)
    {
        if(pos >= LED_WALL_NODE_RENDER_BUFFER_LEN)
            return nullptr;
        return &m_render_buffer[pos];
    }
private:
    static constexpr uint16_t WIDTH_PX  = WALL_AMOUNT_LEDS / WALL_LED_HEIGHT_COUNT;
    static constexpr uint16_t HEIGHT_PX = WALL_LED_HEIGHT_COUNT;

    color_t m_key_colors[WALL_AMOUNT_LEDS];
    uint16_t m_pos_to_led[HEIGHT_PX][WIDTH_PX];
    led_wall_render_node m_render_buffer[LED_WALL_NODE_RENDER_BUFFER_LEN] = {};

    unsigned long m_time_prev = 0;
    uint8_t m_render_buffer_index = 0;
    bool m_render_update = true;
};

}

#endif // LED_WALL_H