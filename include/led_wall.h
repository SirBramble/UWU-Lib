#ifndef LED_WALL_H
#define LED_WALL_H

#include <Adafruit_GFX.h>
#include "color.h"
#include "rgbled.h"
#include "config.h"
#include <AnimatedGIF.h>

#include <SdFat.h>

#include "led_wall_types.h"

namespace uwu
{

struct led_wall_render_node
{
    char text[LED_WALL_NODE_TEXT_BUFFER_LEN] = {'\0'};
    char gif_path[LED_WALL_NODE_GIF_PATH_BUFFER_LEN] =  {'\0'};
    uint8_t gif_fps = 1;
    display_color_effect_t color_effect = display_color_effect_t::NONE;
    led_wall_render_mode render_mode = led_wall_render_mode::TEXT;
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

    void set_flash_volume(FatVolume* flash_volume){m_flash_volume = flash_volume;}

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

    // Gif Section
    bool load_gif_to_psram(char* path);
    void unload_gif_from_psram();
    static void gif_draw_cb_trampoline(GIFDRAW *pDraw);
    void gif_draw_cb(GIFDRAW *pDraw);

    AnimatedGIF m_gif;
    unsigned long m_gif_time_next_frame = 0;
    int m_gif_current_frame_delay = 0;
    uint8_t* m_gif_data = nullptr;
    size_t m_gif_file_size = 0;
    bool m_gif_loaded_to_psram = false;

    // Volume pointer storage
    FatVolume* m_flash_volume = nullptr;
};

}

#endif // LED_WALL_H