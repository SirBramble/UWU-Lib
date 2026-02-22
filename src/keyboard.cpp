#include "keyboard.h"

using namespace uwu;

bool keyboard::init()
{

#if IS_MCU_VERSION != 0
    for (int i = 0; i < KEYBAORD_NUM_ROWS; i++)
        pinMode(this->c_keyboard_row_pins[i], INPUT);

    for (int i = 0; i < KEYBAORD_NUM_COLS; i++)
        pinMode(this->c_keyboard_col_pins[i], INPUT_PULLDOWN);
#endif
    return module::init();
}

void keyboard::update()
{
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

#if IS_MCU_VERSION != 0
void keyboard::update()
{

    for (int row = 0; row < AMOUNT_ROWS; row++)
    {
    pin_mode(this->pins_row[row], OUTPUT);
    digital_write(this->pins_row[row], HIGH);

    for (int col = AMOUNT_COLS - 1; col >= 0; col--)
    {
        if (c_keyboard_key_remap_table[row][col] > -1)
        {
          key* k = get_key(c_keyboard_key_remap_table[row][col]);
          k->update(digital_read(this->pins_col[col]))
        }
    }
    digital_write(this->pins_row[row], LOW);
    pin_mode(this->pins_row[row], INPUT);
    delay(1);
    }
}

void keyboard::pin_mode(uint8_t pin, uint8_t mode)
{
    if(pin & IS_EXPANDER_PIN)
        m_expander->pin_mode(pin, mode);
    else
        pinMode(pin, mode);
}

void keyboard::digital_write(uint8_t pin, uint8_t mode)
{
    if(pin & IS_EXPANDER_PIN)
        m_expander->digital_write(pin, mode);
    else
        digitalWrite(pin, mode);
}

bool keyboard::digital_read(uint8_t pin)
{
    if(pin & EGP0 || pin & EGP1)
        m_expander->digital_read(pin);
    else
        return digitalRead(pin);
}
#endif
