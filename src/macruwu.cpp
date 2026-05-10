#include "macruwu.h"

#include "sender.h"

using namespace uwu;

bool macruwu::init()
{

#if IS_MCU_VERSION != 0
    for (int i = 0; i < MACRUWU_NUM_KEYS; i++)
        pinMode(c_macruwu_key_to_pin_table[i], INPUT);
    
    // uwu::init_sender(HID_USB_CONNECTION_TIMEOUT);
    m_rgbled->init(get_key_colors());
#endif
    return module::init();
}
#if IS_MCU_VERSION == 0
void macruwu::update()
{
    module::update_layer_color_effect();
    return;
}

void macruwu::pin_mode(uint8_t pin, uint8_t mode)
{
    return;
}

void macruwu::digital_write(uint8_t pin, uint8_t mode)
{
    return;
}

bool macruwu::digital_read(uint8_t pin)
{
    return false;
}

#else
void macruwu::update()
{
    for (int i = 0; i < MACRUWU_NUM_KEYS; i++)
    {
        key* k = get_key(i);
        if(k == nullptr)
        {
            Serial.println("NULLUS MULLUS");
            continue;
        }
        key_state_t state = k->update(this->digital_read(c_macruwu_key_to_pin_table[i]));
        if(k->has_layer_change() && state == key_state_t::PRESSED_TRANSITION)
            m_current_layer = k->get_target_layer();
        // delay(1);
    }
    // TODO: Put in millis if or something so that color update is only called every x ms and not every time update() is called
}

void macruwu::pin_mode(uint8_t pin, PinMode mode)
{
    if(pin & IS_EXPANDER_PIN)
        m_expander->pin_mode(pin, mode);
    else
        pinMode(pin, mode);
}

void macruwu::digital_write(uint8_t pin, PinStatus state)
{
    if(pin & IS_EXPANDER_PIN)
        m_expander->digital_write(pin, state);
    else
        digitalWrite(pin, state);
}

bool macruwu::digital_read(uint8_t pin)
{
    if(pin & IS_EXPANDER_PIN)
    {
        return m_expander->digital_read(pin);
    }
    else
        return digitalRead(pin);
}
#endif