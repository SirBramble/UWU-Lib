#include "kts1622.h"
#if IS_MCU_VERSION == 0
#include <cstdio>
#endif

kts1622::kts1622(uint8_t address, uint8_t int_pin, TwoWire* interface)
{
    m_address = address;
    m_irq_pin = int_pin;
    m_wire = interface;
}

void kts1622::use_irq()
{
    write_register(c_addr_reg_irq_mask_0, 0x00); // Enable IRQ
    write_register(c_addr_reg_irq_mask_1, 0x00); // Enable IRQ

    m_using_irq = true;
}

void kts1622::pin_mode(uint8_t pin, PinMode mode)
{
    uint8_t offset = pin & EGP1 ? 0x01 : 0x00;
    uint8_t _pin = pin & 0x07;

    // TODO: Implement other modes

    switch (mode)
    {
        case INPUT:
            write_bit(c_addr_reg_config_0+offset, _pin, 1);
            break;
        case OUTPUT:
            write_bit(c_addr_reg_config_0+offset, _pin, 0);
            break;
        case INPUT_PULLUP:
            write_bit(c_addr_reg_config_0+offset, _pin, 1);
            write_bit(c_addr_reg_pu_pd_select_0+offset, _pin, 1);
            write_bit(c_addr_reg_pu_pd_enable_0+offset, _pin, 1);
            break;
        case INPUT_PULLDOWN:
            write_bit(c_addr_reg_config_0+offset, _pin, 1);
            write_bit(c_addr_reg_pu_pd_select_0+offset, _pin, 0);
            write_bit(c_addr_reg_pu_pd_enable_0+offset, _pin, 1);
            break;
        default:
            break;
    }
}

void kts1622::digital_write(uint8_t pin, PinStatus status)
{
    uint8_t offset = pin & EGP1 ? 0x01 : 0x00;
    uint8_t _pin = pin & 0x07;

    if(status == HIGH)
        write_bit(c_addr_reg_output_0+offset, _pin, 1);
    else
        write_bit(c_addr_reg_output_0+offset, _pin, 0);

}

bool kts1622::digital_read(uint8_t pin)
{
    uint8_t offset = pin & EGP1 ? 0x01 : 0x00;
    uint8_t _pin = pin & 0x07;

    if(is_irq_set())
        update_input_reg(); // Reading from the "Input Port" registers (0x00 and 0x01) clears all pending IRQ

    return m_input_reg[offset] & (1 << _pin);
}

void kts1622::update_input_reg()
{
    m_input_reg[0] = read_register(c_addr_reg_input_0);
    m_input_reg[1] = read_register(c_addr_reg_input_1);
}

void kts1622::write_bit(uint8_t reg, uint8_t bit, bool value)
{
    uint8_t reg_val = read_register(reg);
    if(value)
        reg_val |= (1 << bit);
    else
        reg_val &= ~(1 << bit);

    write_register(reg, reg_val);
}

bool kts1622::read_bit(uint8_t reg, uint8_t bit)
{
    uint8_t reg_val = read_register(reg);
    return reg_val & (1 << bit);
}

#if IS_MCU_VERSION != 0

void kts1622::write_register(uint8_t reg, uint8_t value)
{
    m_wire->beginTransmission(m_address);
    m_wire->write(reg);
    m_wire->write(value);
    m_wire->endTransmission();
}

uint8_t kts1622::read_register(uint8_t reg)
{
    m_wire->beginTransmission(m_address);
    m_wire->write(reg);
    m_wire->endTransmission();
    m_wire->requestFrom(m_address, 1, true);

    if (Wire.available() != 0)
    {
      return Wire.read();
    }
    return 0x00;
}

bool kts1622::is_irq_set()
{
    return digitalRead(m_irq_pin);
}

#else
void kts1622::write_register(uint8_t reg, uint8_t value)
{
    printf("kts1622 reg_write: reg: 0x%02X\t val:0x%02X\n",reg, value);
    return;
}

uint8_t kts1622::read_register(uint8_t reg)
{
    printf("kts1622 reg_read: reg: 0x%02X\t returns: 0x00\n",reg);
    return 0x00;
}

bool kts1622::is_irq_set()
{
    return true;
}

#endif
