#ifndef ANIMACRO_H
#define ANIMACRO_H

#include "color.h"
#include "led_wall_types.h"
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
    COMMAND_B, COMMAND_BA, COMMAND_BAC, COMMAND_BACK, COMMAND_BACKS, COMMAND_BACKSP, COMMAND_BACKSPA, COMMAND_BACKSPAC, COMMAND_BACKSPACE,
    COMMAND_D, COMMAND_DO, COMMAND_DOW, COMMAND_DOWN, COMMAND_DOWN_, COMMAND_DOWN_K, COMMAND_DOWN_KE, COMMAND_DOWN_KEY, COMMAND_DOWN_KEY_BR_OPEN, COMMAND_DOWN_KEY_BR_CLOSE,
               COMMAND_DE, COMMAND_DEL, COMMAND_DELA, COMMAND_DELAY, COMMAND_DELAY_BR_OPEN, COMMAND_DELAY_BR_CLOSE,
    COMMAND_E, COMMAND_ES, COMMAND_ESC,
               COMMAND_EN, COMMAND_ENT, COMMAND_ENTE, COMMAND_ENTER,
    COMMAND_F, COMMAND_F1, COMMAND_F10,
                           COMMAND_F11,
                           COMMAND_F12,
                           COMMAND_F13,
                           COMMAND_F14,
                           COMMAND_F15,
                           COMMAND_F16,
                           COMMAND_F17,
                           COMMAND_F18,
                           COMMAND_F19,
               COMMAND_F2, COMMAND_F20,
                           COMMAND_F21,
                           COMMAND_F22,
                           COMMAND_F23,
                           COMMAND_F24,
               COMMAND_F3,
               COMMAND_F4,
               COMMAND_F5,
               COMMAND_F6,
               COMMAND_F7,
               COMMAND_F8,
               COMMAND_F9,
    COMMAND_K, COMMAND_KE, COMMAND_KEY, COMMAND_KEY_BR_OPEN, COMMAND_KEY_BR_OPEN_HEX, COMMAND_KEY_BR_OPEN_DEC, COMMAND_KEY_BR_OPEN_MOD, COMMAND_KEY_BR_OPEN_MOD_HEX, COMMAND_KEY_BR_OPEN_MOD_DEC, COMMAND_KEY_BR_OPEN_NEXT, COMMAND_KEY_BR_CLOSE,
    COMMAND_L, COMMAND_L_, COMMAND_L_A, COMMAND_L_AL, COMMAND_L_ALT,
                           COMMAND_L_K, COMMAND_L_KE, COMMAND_L_KEY, COMMAND_L_KEY_BR_OPEN, COMMAND_L_KEY_BR_CLOSE,
                           COMMAND_L_M, COMMAND_L_ME, COMMAND_L_MET, COMMAND_L_META,
                           COMMAND_L_S, COMMAND_L_SH, COMMAND_L_SHI, COMMAND_L_SHIF, COMMAND_L_SHIFT,
                                        COMMAND_L_ST, COMMAND_L_STR, COMMAND_L_STRG,
    COMMAND_N, COMMAND_NU, COMMAND_NUL, COMMAND_NULL,
    COMMAND_R, COMMAND_R_, COMMAND_R_A, COMMAND_R_AL, COMMAND_R_ALT,
                           COMMAND_R_K, COMMAND_R_KE, COMMAND_R_KEY, COMMAND_R_KEY_BR_OPEN, COMMAND_R_KEY_BR_CLOSE,
                           COMMAND_R_M, COMMAND_R_ME, COMMAND_R_MET, COMMAND_R_META,
                           COMMAND_R_S, COMMAND_R_SH, COMMAND_R_SHI, COMMAND_R_SHIF, COMMAND_R_SHIFT,
                                        COMMAND_R_ST, COMMAND_R_STR, COMMAND_R_STRG,
    COMMAND_S, COMMAND_ST, COMMAND_STR, COMMAND_STRG, COMMAND_STRG_BR_OPEN,
               COMMAND_SZ,
    COMMAND_U, COMMAND_UP, COMMAND_UP_, COMMAND_UP_K, COMMAND_UP_KE, COMMAND_UP_KEY, COMMAND_UP_KEY_BR_OPEN, COMMAND_UP_KEY_BR_CLOSE,
};

class animacro_parser
{
public:
    bool am_data_parse(const char* c_str, key* k);
    bool am_color_parse(const char* c_str, color_t* color);
    bool am_layer_color_effect_parse(const char* c_str, layer_color_effect_t* effect);
    bool am_display_color_effect_parse(const char* c_str, display_color_effect_t* effect);
    bool am_display_render_mode_parse(const char* c_str, led_wall_render_mode* mode);
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
    bool append_delay_node(uint32_t delay_us);

    uint8_t ascii_to_keycode(char c){return (*m_ascii_conv_table_locale)[c][1];}
    uint8_t ascii_to_mod(char c){return (*m_ascii_conv_table_locale)[c][0];}

};

}


#endif
