#ifndef ANIMACRO_H
#define ANIMACRO_H

#include "color.h"
#include "key.h"
#include "keycodes.h"
#include "config.h"

#if IS_MCU_VERSION == 0
    #include <cstddef>
#else
    #include <Arduino.h>
#endif

namespace uwu {

enum am_state_t
{
    IDLE = 0,
    DONE,
    ERROR,
    COMMAND,
    COMMAND_BACKSLASH,
    COMMAND_L, COMMAND_L_, COMMAND_L_K, COMMAND_L_KE, COMMAND_L_KEY, COMMAND_L_KEY_BR_OPEN, COMMAND_L_KEY_BR_CLOSE,
    COMMAND_N, COMMAND_NU, COMMAND_NUL, COMMAND_NULL,
    COMMAND_S, COMMAND_ST, COMMAND_STR, COMMAND_STRG, COMMAND_STRG_BR_OPEN,
};

class animacro_parser
{
public:
    bool am_data_parse(const char* c_str, key* k);
    bool am_color_parse(const char* c_str, color_t* color);
    bool am_layer_color_effect_parse(const char* c_str, layer_color_effect_t* effect);
    bool am_display_color_effect_parse(const char* c_str, display_color_effect_t* effect);
    bool am_key_color_effect_parse(const char* c_str, key_color_effect_t* effect);
private:
    am_state_t m_am_state = IDLE;

    const char* m_am_str = nullptr;
    size_t m_am_pos = 0;
    key* m_am_key = nullptr;

    // Value Storage
    uint32_t m_num_1 = 0;
    uint32_t m_num_2 = 0;
    uint32_t m_num_3 = 0;
    uint32_t m_num_4 = 0;
    uint32_t m_num_5 = 0;
    uint32_t m_num_6 = 0;

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
