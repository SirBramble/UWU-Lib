#include "telemetry.h"

using namespace uwu;

telemetry::telemetry(kts1622* expander2) :
    m_expander2(expander2)
{
}

void telemetry::init()
{
#if MODULUWU_VERSION >= 2
    m_expander2->use_manual();
#endif
    analogReadResolution(12);
}

bool telemetry::is_pwr_on()
{
#if MODULUWU_VERSION >= 2
    m_expander2->update_input_reg();
    return !m_expander2->digital_read(PIN_PWR_ON_DET);
#else
    return false;
#endif
}

bool telemetry::is_5v_in()
{
#if MODULUWU_VERSION >= 2
    return digitalRead(PIN_5V_IN_DET);
#else
    return true;
#endif
}

bool telemetry::is_usb_in()
{
#if MODULUWU_VERSION >= 2
    m_expander2->update_input_reg();
    return m_expander2->digital_read(PIN_USB_DET);
#else
    return true;
#endif
}

bool telemetry::is_ext_in()
{
#if MODULUWU_VERSION >= 2
    m_expander2->update_input_reg();
    return m_expander2->digital_read(PIN_EXT_DET);
#else
    return false;
#endif
}

uint8_t telemetry::get_battery_percent()
{
    int raw = analogRead(PIN_V_SENSE);
    float voltage = raw * VREF / ADC_MAX;

    if (voltage <= MIN_VOLTAGE)
        return 0.0;

    if (voltage >= MAX_VOLTAGE)
        return 100.0;

    return (voltage - MIN_VOLTAGE) * 100.0 / (MAX_VOLTAGE - MIN_VOLTAGE);
}