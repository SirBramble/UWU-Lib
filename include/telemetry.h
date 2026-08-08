#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "kts1622.h"

#define PIN_PWR_ON_DET EGP0_0
#define PIN_5V_IN_DET 0x40 + 2
#define PIN_USB_DET EGP1_7
#define PIN_EXT_DET EGP0_1

#define PIN_V_SENSE 47

const float ADC_MAX = 4095.0;
const float VREF = 3.3;

const float MIN_VOLTAGE = 2.0625;   // 0%
const float MAX_VOLTAGE = 2.8875;   // 100%

namespace uwu
{

class telemetry
{
public:
    explicit telemetry(kts1622* expander2);

    void init();

    bool is_pwr_on();
    bool is_5v_in();
    bool is_usb_in();
    bool is_ext_in();

    uint8_t get_battery_percent();

private:
    kts1622* m_expander2 = nullptr;
};

}


#endif // TELEMETRY_H