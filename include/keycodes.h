#ifndef KEYCODES_H
#define KEYCODES_H
//================================================================================
//================================================================================
//  Keyboard

// Modifiers
#include <cstdint>
#include <class/hid/hid.h>

#define KEY_MOD_LCTRL     0x01
#define KEY_MOD_LSHIFT    0x02
#define KEY_MOD_LALT      0x04
#define KEY_MOD_LMETA     0x08
#define KEY_MOD_RCTRL     0x10
#define KEY_MOD_RSHIFT    0x20
#define KEY_MOD_RALT      0x40
#define KEY_MOD_RMETA     0x80


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
    {KEY_MOD_RALT   , 0x2D  }, /* 0x2D '\'       */
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
