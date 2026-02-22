#ifndef KEYCODES_H
#define KEYCODES_H
//================================================================================
//================================================================================
//  Keyboard

// Modifiers
#include <cstdint>

#define KEY_MOD_LCTRL     0x01
#define KEY_MOD_LSHIFT    0x02
#define KEY_MOD_LALT      0x04
#define KEY_MOD_LMETA     0x08
#define KEY_MOD_RCTRL     0x10
#define KEY_MOD_RSHIFT    0x20
#define KEY_MOD_RALT      0x40
#define KEY_MOD_RMETA     0x80

#define KEY_LEFT_CTRL     0x00
#define KEY_LEFT_SHIFT    0x01
#define KEY_LEFT_ALT      0x02
#define KEY_LEFT_GUI      0x03
#define KEY_RIGHT_CTRL    0x04
#define KEY_RIGHT_SHIFT   0x05
#define KEY_RIGHT_ALT     0x06
#define KEY_RIGHT_GUI     0x07

// Misc keys
#define KEY_UP_ARROW      0xDA
#define KEY_DOWN_ARROW    0xD9
#define KEY_LEFT_ARROW    0xD8
#define KEY_RIGHT_ARROW   0xD7
#define KEY_BACKSPACE     0xB2
#define KEY_TAB           0xB3
#define KEY_RETURN        0xB0
#define KEY_MENU          0xED // "Keyboard Application" in USB standard
#define KEY_ESC           0xB1
#define KEY_INSERT        0xD1
#define KEY_DELETE        0xD4
#define KEY_PAGE_UP       0xD3
#define KEY_PAGE_DOWN     0xD6
#define KEY_HOME          0xD2
#define KEY_END           0xD5
#define KEY_CAPS_LOCK     0xC1
#define KEY_PRINT_SCREEN  0xCE // Print Screen / SysRq
#define KEY_SCROLL_LOCK   0xCF
#define KEY_PAUSE         0xD0 // Pause / Break

// Numeric keypad
#define KEY_NUM_LOCK      0xDB
#define KEY_KP_SLASH      0xDC
#define KEY_KP_ASTERISK   0xDD
#define KEY_KP_MINUS      0xDE
#define KEY_KP_PLUS       0xDF
#define KEY_KP_ENTER      0xE0
#define KEY_KP_1          0xE1
#define KEY_KP_2          0xE2
#define KEY_KP_3          0xE3
#define KEY_KP_4          0xE4
#define KEY_KP_5          0xE5
#define KEY_KP_6          0xE6
#define KEY_KP_7          0xE7
#define KEY_KP_8          0xE8
#define KEY_KP_9          0xE9
#define KEY_KP_0          0xEA
#define KEY_KP_DOT        0xEB

// Function keys
#define KEY_F1            0xC2
#define KEY_F2            0xC3
#define KEY_F3            0xC4
#define KEY_F4            0xC5
#define KEY_F5            0xC6
#define KEY_F6            0xC7
#define KEY_F7            0xC8
#define KEY_F8            0xC9
#define KEY_F9            0xCA
#define KEY_F10           0xCB
#define KEY_F11           0xCC
#define KEY_F12           0xCD
#define KEY_F13           0xF0
#define KEY_F14           0xF1
#define KEY_F15           0xF2
#define KEY_F16           0xF3
#define KEY_F17           0xF4
#define KEY_F18           0xF5
#define KEY_F19           0xF6
#define KEY_F20           0xF7
#define KEY_F21           0xF8
#define KEY_F22           0xF9
#define KEY_F23           0xFA
#define KEY_F24           0xFB


// Cpnsumer keys, taken from TinyUSB

// Power Control
#define KEY_POWER                              0x0030
#define KEY_RESET                              0x0031
#define KEY_SLEEP                              0x0032

// Screen Brightness
#define KEY_BRIGHTNESS_INCREMENT               0x006F
#define KEY_BRIGHTNESS_DECREMENT               0x0070

// These HID usages operate only on mobile systems (battery powered) and
// require Windows 8 (build 8302 or greater).
#define KEY_WIRELESS_RADIO_CONTROLS            0x000C
#define KEY_WIRELESS_RADIO_BUTTONS             0x00C6
#define KEY_WIRELESS_RADIO_LED                 0x00C7
#define KEY_WIRELESS_RADIO_SLIDER_SWITCH       0x00C8

// Media Control
#define KEY_PLAY_PAUSE                         0x00CD
#define KEY_SCAN_NEXT                          0x00B5
#define KEY_SCAN_PREVIOUS                      0x00B6
#define KEY_STOP                               0x00B7
#define KEY_VOLUME                             0x00E0
#define KEY_MUTE                               0x00E2
#define KEY_BASS                               0x00E3
#define KEY_TREBLE                             0x00E4
#define KEY_BASS_BOOST                         0x00E5
#define KEY_VOLUME_INCREMENT                   0x00E9
#define KEY_VOLUME_DECREMENT                   0x00EA
#define KEY_BASS_INCREMENT                     0x0152
#define KEY_BASS_DECREMENT                     0x0153
#define KEY_TREBLE_INCREMENT                   0x0154
#define KEY_TREBLE_DECREMENT                   0x0155

// Application Launcher
#define KEY_AL_CONSUMER_CONTROL_CONFIGURATION  0x0183
#define KEY_AL_EMAIL_READER                    0x018A
#define KEY_AL_CALCULATOR                      0x0192
#define KEY_AL_LOCAL_BROWSER                   0x0194

// Browser/Explorer Specific
#define KEY_AC_SEARCH                          0x0221
#define KEY_AC_HOME                            0x0223
#define KEY_AC_BACK                            0x0224
#define KEY_AC_FORWARD                         0x0225
#define KEY_AC_STOP                            0x0226
#define KEY_AC_REFRESH                         0x0227
#define KEY_AC_BOOKMARKS                       0x022A

// Mouse Horizontal scroll
#define KEY_AC_PAN                             0x0238


// Supported keyboard layouts
extern const uint8_t KeyboardLayout_de_DE[];
extern const uint8_t KeyboardLayout_en_US[];
extern const uint8_t KeyboardLayout_es_ES[];
extern const uint8_t KeyboardLayout_fr_FR[];
extern const uint8_t KeyboardLayout_it_IT[];
extern const uint8_t KeyboardLayout_pt_PT[];
extern const uint8_t KeyboardLayout_sv_SE[];
extern const uint8_t KeyboardLayout_da_DK[];


/**
 * @brief Remapping table for mapping ASCII characters to keyboard inputs (modifiers and keycodes).
 * @details The current version only supports a german keyboard layout. Future Versions may support multiple layouts.
 * If a different layout is required, the macro can be copied, renamed and modified.
 * The modified Version can then be inserted in the 'ASCII_conv_table'.
 *
 */
// clang-format off
constexpr uint8_t ascii_conv_table_de_DE[128][2] =
{
    {0              , 0     }, /* 0x00 Null      */
    {0              , 0     }, /* 0x01           */
    {0              , 0     }, /* 0x02           */
    {0              , 0     }, /* 0x03           */
    {0              , 0     }, /* 0x04           */
    {0              , 0     }, /* 0x05           */
    {0              , 0     }, /* 0x06           */
    {0              , 0     }, /* 0x07           */
    {0              , 0x2A  }, /* 0x08 Backspace */
    {0              , 0x2B  }, /* 0x09 Tab       */
    {0              , 0x28  }, /* 0x0A Line Feed */
    {0              , 0     }, /* 0x0B           */
    {0              , 0     }, /* 0x0C           */
    {0              , 0x28  }, /* 0x0D CR        */
    {0              , 0     }, /* 0x0E           */
    {0              , 0     }, /* 0x0F           */
    {0              , 0     }, /* 0x10           */
    {0              , 0     }, /* 0x11           */
    {0              , 0     }, /* 0x12           */
    {0              , 0     }, /* 0x13           */
    {0              , 0     }, /* 0x14           */
    {0              , 0     }, /* 0x15           */
    {0              , 0     }, /* 0x16           */
    {0              , 0     }, /* 0x17           */
    {0              , 0     }, /* 0x18           */
    {0              , 0     }, /* 0x19           */
    {0              , 0     }, /* 0x1A           */
    {0              , 0x29  }, /* 0x1B Escape    */
    {0              , 0     }, /* 0x1C           */
    {0              , 0     }, /* 0x1D           */
    {0              , 0     }, /* 0x1E           */
    {0              , 0     }, /* 0x1F           */

    {0              , 0x2C  }, /* 0x20           */
    {KEY_MOD_LSHIFT , 0x1E  }, /* 0x21 !         */
    {KEY_MOD_LSHIFT , 0x1F  }, /* 0x22 "         */
    {0              , 0x32  }, /* 0x23 #         */
    {KEY_MOD_LSHIFT , 0x21  }, /* 0x24 $         */
    {KEY_MOD_LSHIFT , 0x22  }, /* 0x25 %         */
    {KEY_MOD_LSHIFT , 0x23  }, /* 0x26 &         */
    {KEY_MOD_LSHIFT , 0x32  }, /* 0x27 '         */
    {KEY_MOD_LSHIFT , 0x25  }, /* 0x28 (         */
    {KEY_MOD_LSHIFT , 0x26  }, /* 0x29 )         */
    {KEY_MOD_LSHIFT , 0x30  }, /* 0x2A *         */
    {0              , 0x30  }, /* 0x2B +         */
    {0              , 0x36  }, /* 0x2C ,         */
    {0              , 0x38  }, /* 0x2D -         */
    {0              , 0x37  }, /* 0x2E .         */
    {KEY_MOD_LSHIFT , 0x24  }, /* 0x2F /         */
    {0              , 0x27  }, /* 0x30 0         */
    {0              , 0x1E  }, /* 0x31 1         */
    {0              , 0x1F  }, /* 0x32 2         */
    {0              , 0x20  }, /* 0x33 3         */
    {0              , 0x21  }, /* 0x34 4         */
    {0              , 0x22  }, /* 0x35 5         */
    {0              , 0x23  }, /* 0x36 6         */
    {0              , 0x24  }, /* 0x37 7         */
    {0              , 0x25  }, /* 0x38 8         */
    {0              , 0x26  }, /* 0x39 9         */
    {KEY_MOD_LSHIFT , 0x37  }, /* 0x3A :         */
    {KEY_MOD_LSHIFT , 0x36  }, /* 0x3B ;         */
    {0              , 0x64  }, /* 0x3C <         */
    {KEY_MOD_LSHIFT , 0x27  }, /* 0x3D =         */
    {KEY_MOD_LSHIFT , 0x64  }, /* 0x3E >         */
    {KEY_MOD_LSHIFT , 0x2D  }, /* 0x3F ? may be 0x2E*/

    {KEY_MOD_LSHIFT , 0x1F  }, /* 0x40 @         */
    {KEY_MOD_LSHIFT , 0x04  }, /* 0x41 A         */
    {KEY_MOD_LSHIFT , 0x05  }, /* 0x42 B         */
    {KEY_MOD_LSHIFT , 0x06  }, /* 0x43 C         */
    {KEY_MOD_LSHIFT , 0x07  }, /* 0x44 D         */
    {KEY_MOD_LSHIFT , 0x08  }, /* 0x45 E         */
    {KEY_MOD_LSHIFT , 0x09  }, /* 0x46 F         */
    {KEY_MOD_LSHIFT , 0x0A  }, /* 0x47 G         */
    {KEY_MOD_LSHIFT , 0x0B  }, /* 0x48 H         */
    {KEY_MOD_LSHIFT , 0x0C  }, /* 0x49 I         */
    {KEY_MOD_LSHIFT , 0x0D  }, /* 0x4A J         */
    {KEY_MOD_LSHIFT , 0x0E  }, /* 0x4B K         */
    {KEY_MOD_LSHIFT , 0x0F  }, /* 0x4C L         */
    {KEY_MOD_LSHIFT , 0x10  }, /* 0x4D M         */
    {KEY_MOD_LSHIFT , 0x11  }, /* 0x4E N         */
    {KEY_MOD_LSHIFT , 0x12  }, /* 0x4F O         */
    {KEY_MOD_LSHIFT , 0x13  }, /* 0x50 P         */
    {KEY_MOD_LSHIFT , 0x14  }, /* 0x51 Q         */
    {KEY_MOD_LSHIFT , 0x15  }, /* 0x52 R         */
    {KEY_MOD_LSHIFT , 0x16  }, /* 0x53 S         */
    {KEY_MOD_LSHIFT , 0x17  }, /* 0x55 T         */
    {KEY_MOD_LSHIFT , 0x18  }, /* 0x55 U         */
    {KEY_MOD_LSHIFT , 0x19  }, /* 0x56 V         */
    {KEY_MOD_LSHIFT , 0x1A  }, /* 0x57 W         */
    {KEY_MOD_LSHIFT , 0x1B  }, /* 0x58 X         */
    {KEY_MOD_LSHIFT , 0x1D  }, /* 0x59 Y         */
    {KEY_MOD_LSHIFT , 0x1C  }, /* 0x5A Z         */
    {KEY_MOD_RALT   , 0x25  }, /* 0x5B [         */
    {KEY_MOD_RALT   , 0x2E  }, /* 0x5C '\'       */
    {KEY_MOD_RALT   , 0x26  }, /* 0x5D ]         */
    {0              , 0x35  }, /* 0x5E ^         */
    {KEY_MOD_LSHIFT , 0x38  }, /* 0x5F _         */

    {0              , 0x35  }, /* 0x60 `         */
    {0              , 0x04  }, /* 0x61 a         */
    {0              , 0x05  }, /* 0x62 b         */
    {0              , 0x06  }, /* 0x63 c         */
    {0              , 0x07  }, /* 0x66 d         */
    {0              , 0x08  }, /* 0x65 e         */
    {0              , 0x09  }, /* 0x66 f         */
    {0              , 0x0A  }, /* 0x67 g         */
    {0              , 0x0B  }, /* 0x68 h         */
    {0              , 0x0C  }, /* 0x69 i         */
    {0              , 0x0D  }, /* 0x6A j         */
    {0              , 0x0E  }, /* 0x6B k         */
    {0              , 0x0F  }, /* 0x6C l         */
    {0              , 0x10  }, /* 0x6D m         */
    {0              , 0x11  }, /* 0x6E n         */
    {0              , 0x12  }, /* 0x6F o         */
    {0              , 0x13  }, /* 0x70 p         */
    {0              , 0x14  }, /* 0x71 q         */
    {0              , 0x15  }, /* 0x72 r         */
    {0              , 0x16  }, /* 0x73 s         */
    {0              , 0x17  }, /* 0x75 t         */
    {0              , 0x18  }, /* 0x75 u         */
    {0              , 0x19  }, /* 0x76 v         */
    {0              , 0x1A  }, /* 0x77 w         */
    {0              , 0x1B  }, /* 0x78 x         */
    {0              , 0x1D  }, /* 0x79 y         */
    {0              , 0x1C  }, /* 0x7A z         */
    {KEY_MOD_RALT   , 0x24  }, /* 0x7B {         */
    {KEY_MOD_RALT   , 0x64  }, /* 0x7C |         */
    {KEY_MOD_RALT   , 0x27  }, /* 0x7D }         */
    {KEY_MOD_RALT   , 0x30  }, /* 0x7E ~  !may also be 0x31! */
    {0              , 0x4C  }  /* 0x7F Delete    */
};
// clang-format on

#endif // KEYCODES_H
