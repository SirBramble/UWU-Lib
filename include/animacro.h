#ifndef ANIMACRO_H
#define ANIMACRO_H

#include "color.h"
#include "key.h"
#include "keycodes.h"
#include <cstddef>

#ifndef AM_MAX_LOOP
#define AM_MAX_LOOP 200
#endif

namespace uwu {

enum am_state_t
{
    IDLE = 0,
    DONE,
    ERROR,
    COMMAND,
    COMMAND_N, COMMAND_NU, COMMAND_NUL, COMMAND_NULL,
    COMMAND_S, COMMAND_ST, COMMAND_STR, COMMAND_STRG,
};

class animacro_parser
{
public:
    bool am_data_parse(const char* c_str, key* k);
    bool am_color_parse(const char* c_str, color_t* color);
    bool am_layer_color_effect_parse(const char* c_str, layer_color_effect_t* effect);
    bool am_key_color_effect_parse(const char* c_str, key_color_effect_t* effect);
private:
    am_state_t m_am_state = IDLE;

    const char* m_am_str;
    size_t m_am_pos = 0;
    key* m_am_key = nullptr;

    // Layout table
    const uint8_t (*m_ascii_conv_table_locale)[128][2] = &ascii_conv_table_de_DE;

    void am_next_char();

    bool append_keycode(char c);
    bool append_keycode(uint8_t keycode, uint8_t mod, uint8_t r_id);
    bool append_null_node(uint8_t r_id);

    uint8_t ascii_to_keycode(char c){return (*m_ascii_conv_table_locale)[c][1];}
    uint8_t ascii_to_mod(char c){return (*m_ascii_conv_table_locale)[c][0];}

};

}


#endif
