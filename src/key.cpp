#include "key.h"
#include "color.h"
#include "sender.h"

using namespace uwu;

bool key::init(color_t *key_color)
{
    m_key_color = key_color;
    free_node(m_root);  // Init can be called several times. If m_root already populated, RESET!
    m_root = uwu::alloc_node();
    if(m_root == nullptr)
        return false;
    return true;
}

void key::update(bool pressed)
{
    switch (m_key_state)
    {
        case RELEASED:
            if (m_color_effect == key_color_effect_t::CONST_COLOR)
            {
                *m_key_color = m_color;
                m_key_color->a = 3;
            }
            else
            {
                m_key_color->a = 0;
            }
            if (pressed)
                m_key_state = RELEASED_TRANSITION;
            break;
        case RELEASED_TRANSITION:
            Serial.printf("1 m_color_effect=%d\tm_key_color->a=%d\t&m_key_color=%p\n",m_color_effect,m_key_color->a,&m_key_color);
            m_key_state = PRESSED;
            m_key_toggled ^= m_key_toggled;
            send_keycode(this->get_keycode_root(), this->is_single());
            if(m_color_effect == key_color_effect_t::PRESSED || m_color_effect == key_color_effect_t::PRESSED_AND_RELEASED)
            {
                *m_key_color = m_color_pressed;
                m_key_color->a = 2;
            }
            else if (m_color_effect == key_color_effect_t::CONST_COLOR)
            {
                *m_key_color = m_color;
                m_key_color->a = 3;
            }
            else
            {
                m_key_color->a = 0;
            }
            Serial.printf("\n2 m_color_effect=%d\tm_key_color->a=%d\n",m_color_effect,m_key_color->a);
            break;
        case PRESSED:
            if (!pressed)
                m_key_state = PRESSED_TRANSITION;
            break;
        case PRESSED_TRANSITION:
            m_key_state = RELEASED;
            clear_keycode(this->get_keycode_root(), this->is_single());
            if(m_color_effect == key_color_effect_t::RELEASED || m_color_effect == key_color_effect_t::PRESSED_AND_RELEASED)
            {
                *m_key_color = m_color_released;
                m_key_color->a = 1;
            }
            else if (m_color_effect == key_color_effect_t::CONST_COLOR)
            {
                *m_key_color = m_color;
                m_key_color->a = 3;
            }
            else
            {
                m_key_color->a = 0;
            }
            break;
        default:
            break;
    }
}

bool key::is_single()
{
    if(m_root->next !=nullptr)
        return false;
    if(m_root->mod != 0)
        return false;
    for(int i = 1; i < 6; i++)
        if(m_root->codes[i] != 0)
            return false;

    return true;
}
