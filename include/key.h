#ifndef KEY_H
#define KEY_H

#include "memory.h"
#include "color.h"

#if (IS_MCU_VERSION == 0)
#include <string>
#endif

namespace uwu
{

enum key_state_t
{
    RELEASED = 0,
    RELEASED_TRANSITION,
    PRESSED,
    PRESSED_TRANSITION
};

    // Let inherit from sender class. Then each key object is responsable for sending its own data and only need a 'update' or 'input' function to tell the key object if the corresponding physical key is pressed.
class key
{
public:
    bool init(color_t *key_color);

    void update(bool pressed);

    keycode_node* get_keycode_root(){return m_root;}
    // Check for send fuction
    bool is_single();   // use to switch between 2 fuctions: send_macro and send_key. Send macro is standalone and overwrites the current key send buffer. Send key appends index 0 of 'key->codes' to the current global send buffer

    // Debug info functions
    bool has_parse_error(){return m_has_parse_error;}
    void set_parse_error(bool state){m_has_parse_error = state;}

    // Color_pressed
    color_t get_color(){return m_color;}
    void set_color(color_t c){m_color = c;};
    void set_color(uint32_t v){m_color = color_t::from_u32(v);};
    void set_color(uint8_t r, uint8_t g, uint8_t b){m_color = {r,g,b,0};};
    void set_color_r(uint8_t r){m_color.r = r;}
    void set_color_g(uint8_t g){m_color.g = g;}
    void set_color_b(uint8_t b){m_color.b = b;}

    // Color_pressed
    color_t get_color_pressed(){return m_color_pressed;}
    void set_color_pressed(color_t c){m_color_pressed = c;};
    void set_color_pressed(uint32_t v){m_color_pressed = color_t::from_u32(v);};
    void set_color_pressed(uint8_t r, uint8_t g, uint8_t b){m_color_pressed = {r,g,b,0};};
    void set_color_pressed_r(uint8_t r){m_color_pressed.r = r;}
    void set_color_pressed_g(uint8_t g){m_color_pressed.g = g;}
    void set_color_pressed_b(uint8_t b){m_color_pressed.b = b;}

    // Color_released
    color_t get_color_released(){return m_color_released;}
    void set_color_released(color_t c){m_color_released = c;};
    void set_color_released(uint32_t v){m_color_released = color_t::from_u32(v);};
    void set_color_released(uint8_t r, uint8_t g, uint8_t b){m_color_released = {r,g,b,0};};
    void set_color_released_r(uint8_t r){m_color_released.r = r;}
    void set_color_released_g(uint8_t g){m_color_released.g = g;}
    void set_color_released_b(uint8_t b){m_color_released.b = b;}

    // Color Mode
    void set_color_effect(key_color_effect_t effect){m_color_effect = effect;}

#if (IS_MCU_VERSION == 0)
    void set_data(char * c_str){m_data = c_str ? c_str : "";}
#endif

private:
    keycode_node* m_root = nullptr;

    bool m_has_parse_error = false;

    key_state_t m_key_state = RELEASED;
    bool m_key_toggled = false;


    color_t m_color = {0,0,0,0};
    color_t m_color_pressed = {0,0,0,0};
    color_t m_color_released = {0,0,0,0};
    color_t *m_key_color = nullptr;             // Use alpha channel to set if key is overriding color. If a==0 then apply layer color. if a!=0 do not override

    key_color_effect_t m_color_effect = key_color_effect_t::NONE;

#if (IS_MCU_VERSION == 0)
    std::string m_data;
#endif
};


}
#endif
