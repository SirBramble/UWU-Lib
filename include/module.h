#ifndef MODULE_H
#define MODULE_H
#include "animacro.h"
#include "layer.h"
#include "led_wall.h"
#include "lwjson.h"
#include "config.h"

#if IS_MCU_VERSION != 0
    #include <SdFat.h>
#endif

namespace uwu
{

class _module : public animacro_parser
{
public:
    _module(const char* name, led_wall* led_wall = nullptr ) :
        m_name(name),
        m_led_wall(led_wall)
    {
        
    }

    void update_keymap(const char* json_str);
#if IS_MCU_VERSION != 0
    void update_keymap_from_file(FatVolume &volume);
#endif

    virtual key* get_key(int key, int layer) = 0;
    virtual bool init() = 0;

protected:
    virtual _layer* get_layer(int layer) = 0;

    const char* m_name;
    lwjson_stream_parser_t m_stream_parser;

    led_wall* m_led_wall;

    static void update_keymap_json_callback(lwjson_stream_parser_t* jsp, lwjson_stream_type_t type);
    void update_keymap_handle_json_callback(lwjson_stream_parser_t* jsp, lwjson_stream_type_t type);

    int m_current_layer = 0;

    int m_parser_layer = 0;
    int m_parser_key = 0;
    int m_parser_display_index = 0;
};

template<std::size_t N>
class module : public _module
{
public:
    module(const char* name, const int* led_remap_table = nullptr, led_wall* led_wall = nullptr ) :
        _module(name, led_wall),
        m_led_remap_table(led_remap_table)
    {
        
    }
    bool init()
    {
        m_initialised = true;
        for(int i = 0; i < MAX_NUM_LAYERS; i++)
        {
            m_initialised = m_layers[i].init(m_key_colors, m_led_remap_table) ? m_initialised : false;
        }

        for(int i = 0; i < N; i++)
        {
            m_key_colors[i] = {0,0,0,0};
        }

        m_led_wall->clear_render_buffer();
        m_led_wall->render_update();

        return m_initialised;
    }

    key* get_key(int key)
    {
        return get_key(key, m_current_layer);
    }

    key* get_key(int key, int layer)
    {
        if(layer < MAX_NUM_LAYERS)
            return m_layers[layer].get_key(key);
        return nullptr;
    }

    _layer* get_layer(int layer)
    {
        if(layer < MAX_NUM_LAYERS)
            return &m_layers[layer];
        return nullptr;
    }

    color_t* get_key_colors(){return m_key_colors;}

    void update_layer_color_effect()
    {
        m_layers[m_current_layer].update_color();
    }

private:
    bool m_initialised = false;
    const int *m_led_remap_table = nullptr;

    layer<N> m_layers[MAX_NUM_LAYERS];
    color_t m_key_colors[N];
};
}



#endif
