#ifndef LAYER_H
#define LAYER_H

#include "color.h"
#include "key.h"

#if IS_MCU_VERSION != 0
    #include <Arduino.h>
#endif

namespace uwu
{

class _layer
{
public:
    // Color
    color_t get_color(){return m_color;}
    void set_color(color_t c){m_color = c;};
    void set_color(uint32_t v){m_color = color_t::from_u32(v);};
    void set_color(uint8_t r, uint8_t g, uint8_t b){m_color = {r,g,b,0};};
    void set_color_r(uint8_t r){m_color.r = r;}
    void set_color_g(uint8_t g){m_color.g = g;}
    void set_color_b(uint8_t b){m_color.b = b;}

    // Effect
    void set_effect(layer_color_effect_t effect){m_effect = effect;}
    layer_color_effect_t get_effect(){return m_effect;}

    // Effect speed
    void set_effect_speed(uint8_t speed){m_effect_speed = speed;}
    uint8_t get_effect_speed(){return m_effect_speed;}

    // Debug
    void print();

protected:
    color_t m_color = {0,0,0,0};
    layer_color_effect_t m_effect = layer_color_effect_t::NONE;
    uint8_t m_effect_speed = 1;
};

template<std::size_t N>
class layer : public _layer
{
public:
    bool init(color_t key_colors[N], const int led_remap_table[N])
    {
        m_key_colors = key_colors;
        for(size_t i = 0; i < N; i++)
        {
            color_t* key_color;
            if(led_remap_table == nullptr)
                key_color = &key_colors[i];
            else if(led_remap_table[i] < N)
                key_color = &key_colors[led_remap_table[i]];

            bool err = !m_keys[i].init(key_color);
            if(err)
                return false;
        }

        return true;
    }
    key* get_key(int key_index)
    {
        if (key_index >= 0 && static_cast<std::size_t>(key_index) < N)
            return &m_keys[key_index];
        return nullptr;
    }

    bool update_color()
    {
        return apply_layer_color_effect(m_effect, m_effect_speed, m_color, m_key_colors, N);
    }

private:
    key m_keys[N]{};
    color_t* m_key_colors = nullptr;
};
}

#endif
