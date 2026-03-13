#include "keyboard.h"
#include "sender.h"

using namespace uwu;

bool keyboard::init()
{

#if IS_MCU_VERSION != 0
    for (int i = 0; i < KEYBAORD_NUM_ROWS; i++)
        pinMode(c_keyboard_row_pins[i], INPUT);

    for (int i = 0; i < KEYBAORD_NUM_COLS; i++)
        pinMode(c_keyboard_col_pins[i], INPUT_PULLDOWN);
    
    // uwu::init_sender(HID_USB_CONNECTION_TIMEOUT);
    m_rgbled->init(get_key_colors());
#endif
    return module::init();
}
#if IS_MCU_VERSION == 0
void keyboard::update()
{
    module::update_layer_color_effect();
    return;
}

void keyboard::pin_mode(uint8_t pin, uint8_t mode)
{
    return;
}

void keyboard::digital_write(uint8_t pin, uint8_t mode)
{
    return;
}

bool keyboard::digital_read(uint8_t pin)
{
    return false;
}

#else
void keyboard::update()
{

    for (int row = 0; row < KEYBAORD_NUM_ROWS; row++)
    {
        pin_mode(c_keyboard_row_pins[row], OUTPUT);
        digital_write(c_keyboard_col_pins[row], HIGH);

        for (int col = KEYBAORD_NUM_COLS - 1; col >= 0; col--)
        {
            if (c_keyboard_key_remap_table[row][col] > -1)
            {
                key* k = get_key(c_keyboard_key_remap_table[row][col]);
                k->update(digital_read(c_keyboard_col_pins[col]));
            }
        }
        digital_write(c_keyboard_row_pins[row], LOW);
        pin_mode(c_keyboard_row_pins[row], INPUT);
        delay(1);
    }
    // TODO: Put in millis if or something so that color update is only called every x ms and not every time update() is called
    module::update_layer_color_effect();
    m_rgbled->send();
}

void keyboard::pin_mode(uint8_t pin, PinMode mode)
{
    if(pin & IS_EXPANDER_PIN)
        m_expander->pin_mode(pin, mode);
    else
        pinMode(pin, mode);
}

void keyboard::digital_write(uint8_t pin, PinStatus state)
{
    if(pin & IS_EXPANDER_PIN)
        m_expander->digital_write(pin, state);
    else
        digitalWrite(pin, state);
}

bool keyboard::digital_read(uint8_t pin)
{
    if(pin & IS_EXPANDER_PIN)
        return m_expander->digital_read(pin);
    else
        return digitalRead(pin);
}
#endif
