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

    m_gif.begin(GIF_PALETTE_RGB565_LE);
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

    unload_gif_from_psram();
}

void led_wall::render_update()
{
    switch(m_render_buffer[m_render_buffer_index].render_mode)
    {
        case led_wall_render_mode::TEXT:
            if(m_render_buffer[m_render_buffer_index].text[0] == '\0')
                return;
            
            clear();
            setCursor(0, 0);   // GFX text uses baseline positioning
            setTextColor(m_render_buffer[m_render_buffer_index].color.to_rgb565());          // white in RGB565
            print(m_render_buffer[m_render_buffer_index].text);

            m_time_prev = millis();
            break;
        case led_wall_render_mode::GIF:
            if(m_render_buffer[m_render_buffer_index].gif_path[0] == '\0')
                return;

            // If GIF for the current index is not loaded, load it
            if(!m_gif_loaded_to_psram)
            {
                m_gif_loaded_to_psram = load_gif_to_psram(m_render_buffer[m_render_buffer_index].gif_path);
                // If copy to PSRAM socceded, open m_gif with data
                if(m_gif_loaded_to_psram)
                {
                    m_gif.open(m_gif_data, m_gif_file_size, gif_draw_cb_trampoline);    // Errors on opening/parsing the file should be handled internally by the lib...
                    clear();    // clear screen
                }
            }

            // Only play frame, if a frame was loaded
            if(m_gif_loaded_to_psram)
                m_gif.playFrame(false, &m_gif_current_frame_delay, this);
        
            break;
        default:
            break;
    }
    
}

void led_wall::set_pixel(uint16_t x, uint16_t y, color_t color)
{
    if(x < 0 || y < 0 || x >= WIDTH_PX || y >= HEIGHT_PX)
        return;
    m_key_colors[m_pos_to_led[y][x]] = color;
}

void led_wall::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    color_t c;
    c.r = ((color >> 11) & 0x1F) * 255 / 31;
    c.g = ((color >> 5)  & 0x3F) * 255 / 63;
    c.b = ( color        & 0x1F) * 255 / 31;
    c.a = 0;        // Can use this for positive/inverted text color

    set_pixel(x, y, c);
}

void led_wall::update()
{
    switch(m_render_buffer[m_render_buffer_index].render_mode)
    {
        case led_wall_render_mode::TEXT:
            // Initial render update trap
            if (m_render_update)
            {
                m_render_update = false;
                render_update();
            }
            break;
        case led_wall_render_mode::GIF:
            // Trigger GIF Render update
            if( m_render_update || millis() >= m_gif_time_next_frame)
            {
                m_render_update = false;
                render_update();
                m_gif_time_next_frame = millis() + m_gif_current_frame_delay;
            }
            break;
        default:
            break;
    }


    if (m_render_buffer[m_render_buffer_index].duration >= 0 && (millis() - m_time_prev) >= static_cast<unsigned long>(m_render_buffer[m_render_buffer_index].duration))
    {
        m_time_prev = millis();
        uint8_t start = m_render_buffer_index;
        bool found = false;

        // Unload GIF
        unload_gif_from_psram();

        // Find next valid node
        for (uint8_t step = 1; step < LED_WALL_NODE_RENDER_BUFFER_LEN; ++step)
        {
            uint8_t idx = (start + step) % LED_WALL_NODE_RENDER_BUFFER_LEN; // also search with wrap around to start
            // Different validity checks for different render modes
            switch(m_render_buffer[idx].render_mode)
            {
                case led_wall_render_mode::TEXT:
                    if (m_render_buffer[idx].text[0] != '\0')
                    {
                        m_render_buffer_index = idx;
                        found = true;
                    }
                    break;
                case led_wall_render_mode::GIF:
                    if (m_render_buffer[idx].gif_path[0] != '\0')
                    {
                        m_render_buffer_index = idx;
                        found = true;
                    }
                    break;
                default:
                    break;
            }

            if(found)
                break;
        }

        // If no node is found, something wrong! But we'll ignore that and just go to start...
        if (!found)
        {
            Serial.printf("no valid found!!!\n");
            m_render_buffer_index = 0;
        }

        render_update();

    }

    apply_display_color_effect(m_render_buffer[m_render_buffer_index].color_effect, m_render_buffer[m_render_buffer_index].color_effect_speed, m_render_buffer[m_render_buffer_index].color, m_key_colors, WALL_AMOUNT_LEDS);

    rgbled::send();
}

bool led_wall::load_gif_to_psram(char* path)
{
    if(m_flash_volume == nullptr)
        return false;

    File32 file = m_flash_volume->open(path, O_RDONLY);

    if (!file) {
        Serial.printf("Open file (%s) failed\n", path);
        return false;
    }

    m_gif_file_size = file.size();
    m_gif_data = (uint8_t *)pmalloc(m_gif_file_size);   // PSRAM on arduino-pico
    if (!m_gif_data) {
        file.close();
        return false;
    }

    size_t got = file.read(m_gif_data, m_gif_file_size);
    file.close();

    return got == m_gif_file_size;
}

void led_wall::unload_gif_from_psram()
{
    if (m_gif_data != nullptr)
    {
        m_gif.close();
        free(m_gif_data);
        m_gif_data = nullptr;
        m_gif_file_size = 0;
        m_gif_loaded_to_psram = false;
    }

}

void led_wall::gif_draw_cb_trampoline(GIFDRAW *pDraw)
{
    led_wall *self = static_cast<led_wall *>(pDraw->pUser);
    if (!self) return;

    self->gif_draw_cb(pDraw);
}

void led_wall::gif_draw_cb(GIFDRAW *pDraw)
{
    uint16_t *palette = pDraw->pPalette;
    uint8_t  *pixels  = pDraw->pPixels;

    int y  = pDraw->iY + pDraw->y;
    int x0 = pDraw->iX;

    if (y < 0 || y >= HEIGHT_PX)
        return;

    for (int x = 0; x < pDraw->iWidth; x++)
    {
        int screenX = x0 + x;

        if (screenX < 0 || screenX >= WIDTH_PX)
            continue;

        uint8_t index = pixels[x];

        color_t color;
        uint16_t color565 = palette[index];
        color.r = ((color565 >> 11) & 0x1F) * 255 / 31;
        color.g = ((color565 >> 5)  & 0x3F) * 255 / 63;
        color.b = ( color565        & 0x1F) * 255 / 31;

        // If has transparency, let background effect shine through
        if (pDraw->ucHasTransparency && index == pDraw->ucTransparent)
            color.a = 0;
        else
            color.a = 255;

        set_pixel(screenX, y, color);
    }
}