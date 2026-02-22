#ifndef MODULE_H
#define MODULE_H
#include "animacro.h"
#include "layer.h"
#include "lwjson.h"

#ifndef MAX_NUM_LAYERS
#define MAX_NUM_LAYERS 16
#endif

#ifndef IS_MCU_VERSION
#define IS_MCU_VERSION 1
#endif

namespace uwu
{

class _module : public animacro_parser
{
public:
    _module(const char* name) : m_name(name){}

    void update_keymap(const char* json_str);

    virtual key* get_key(int key, int layer) = 0;

protected:
    virtual _layer* get_layer(int layer) = 0;

    const char* m_name;
    lwjson_stream_parser_t m_stream_parser;

    static void update_keymap_json_callback(lwjson_stream_parser_t* jsp, lwjson_stream_type_t type);
    void update_keymap_handle_json_callback(lwjson_stream_parser_t* jsp, lwjson_stream_type_t type);

    int m_current_layer = 0;

    int m_parser_layer = 0;
    int m_parser_key = 0;
};

template<std::size_t N>
class module : public _module
{
public:
    module(const char* name, const int *led_remap_table = nullptr) : _module(name), m_led_remap_table(led_remap_table){}
    bool init()
    {
        m_initialised = true;
        for(int i = 0; i < MAX_NUM_LAYERS; i++)
        {
            m_initialised = m_layers[i].init(m_key_colors, m_led_remap_table) ? m_initialised : false;
            // std::cout << m_led_remap_table << std::endl;
        }

        for(int i = 0; i < N; i++)
        {
            m_key_colors[i] = {0,0,0,0};
        }

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
