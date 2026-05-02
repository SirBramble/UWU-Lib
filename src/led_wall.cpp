#include "led_wall.h"
#include "color.h"

using namespace uwu;

led_wall::led_wall(uint8_t pin) :
    rgbled(pin, WALL_AMOUNT_LEDS, WALL_LED_MAX_BRIGHTNESS),
    Adafruit_GFX(WIDTH_PX, HEIGHT_PX)
{

}

void led_wall::init()
{
    for(auto & key_color : m_key_colors)
        key_color = {0,0,0,0};

    for(int i = 0; i < WALL_AMOUNT_LEDS/WALL_LED_HEIGHT_COUNT; i++)
    {
        for(int n = 0; n < WALL_LED_HEIGHT_COUNT; n++)
        {
            if(i%2 == 0)
                m_pos_to_led[n][i] = i*WALL_LED_HEIGHT_COUNT + WALL_LED_HEIGHT_COUNT-n-1;
            else
                m_pos_to_led[n][i] = i*WALL_LED_HEIGHT_COUNT + n;
        }
    }

    rgbled::init(m_key_colors);

    clear_render_buffer();

    m_time_prev = millis();

    clear();

    setTextWrap(false);
    setCursor(0, 0);   // GFX text uses baseline positioning
    setTextColor(0xFFFF);          // white in RGB565
    print("MACRUWU 3");
}


void led_wall::clear()
{
    for(auto &key_color : m_key_colors)
        key_color = {0, 0, 0, 255};
}

void led_wall::clear_render_buffer()
{
    for(auto & node : m_render_buffer)
    {
        node = led_wall_render_node{};
    }

    m_render_buffer_index = 0;
    m_render_update = true;
}

void led_wall::render_update()
{
    if(m_render_buffer[m_render_buffer_index].text[0] == '\0')
        return;
    
    clear();
    setCursor(0, 0);   // GFX text uses baseline positioning
    setTextColor(m_render_buffer[m_render_buffer_index].color.to_rgb565());          // white in RGB565
    print(m_render_buffer[m_render_buffer_index].text);

    m_time_prev = millis();
}

void led_wall::set_pixel(uint16_t x, uint16_t y, color_t color)
{
    if(x >= WIDTH_PX || y >= HEIGHT_PX)
        return;
    m_key_colors[m_pos_to_led[y][x]] = color;
}

void led_wall::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if(x < 0 || y < 0 || x >= width() || y >= height())
        return;

    color_t c;
    c.r = ((color >> 11) & 0x1F) * 255 / 31;
    c.g = ((color >> 5)  & 0x3F) * 255 / 63;
    c.b = ( color        & 0x1F) * 255 / 31;
    c.a = 0;        // Can use this for positive/inverted text color

    set_pixel(static_cast<uint16_t>(x), static_cast<uint16_t>(y), c);
}

void led_wall::update()
{
    if (m_render_update)
    {
        m_render_update = false;
        render_update();
    }

    if (m_render_buffer[m_render_buffer_index].duration >= 0 &&
        (millis() - m_time_prev) >= static_cast<unsigned long>(m_render_buffer[m_render_buffer_index].duration))
    {
        uint8_t start = m_render_buffer_index;
        bool found = false;

        for (uint8_t step = 1; step < LED_WALL_NODE_RENDER_BUFFER_LEN; ++step)
        {
            uint8_t idx = (start + step) % LED_WALL_NODE_RENDER_BUFFER_LEN;
            if (m_render_buffer[idx].text[0] != '\0')
            {
                m_render_buffer_index = idx;
                found = true;
                break;
            }
        }

        if (!found)
        {
            m_render_buffer_index = 0;
        }

        m_render_update = true;

        if (m_render_update)
        {
            m_render_update = false;
            render_update();
        }

        m_time_prev = millis();
    }

    apply_display_color_effect(m_render_buffer[m_render_buffer_index].color_effect, m_render_buffer[m_render_buffer_index].color_effect_speed, m_render_buffer[m_render_buffer_index].color, m_key_colors, WALL_AMOUNT_LEDS);

    rgbled::send();
}