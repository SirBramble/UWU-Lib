#if PICO_CYW43_SUPPORTED == 1

#include "sender_ble.h"
#include <PicoBluetoothBLEHID.h>
#include <HID_Bluetooth.h>
#include <Arduino.h>

uint16_t report_size = 0;
uint8_t* report_map = nullptr;

static bool sender_ble_started = false;

void sender_ble_begin()
{
  if(sender_ble_started)
    return;

  __SetupHIDreportmap(
    nullptr,      // mouse callback
    [](){},       // keyboard enabled
    nullptr,      // joystick callback
    false,        // abs mouse
    &report_size,
    &report_map
  );

  bool ok = PicoBluetoothBLEHID.startHID(
    "Macruwu3",               // BLE advertising name
    "Macruwu3 Keyboard",      // HID device name
    __BLEGetAppearance(),     // keyboard appearance
    report_map,
    report_size,
    100                       // battery %
  );

  sender_ble_started = true;
}

bool sender_ble_running()
{
  return sender_ble_started;
}

void sendBleKeyboardReport(uwu::keycode_node* node) {
    if (!node) return;
    if (!PicoBluetoothBLEHID.connected()) return;

  uint8_t report[9] = {
    1,                  // BLE Report ID
    node->mod,          // mod
    0x00,               // reserved
    node->codes[0],
    node->codes[1],
    node->codes[2],
    node->codes[3],
    node->codes[4],
    node->codes[5]
  };

  PicoBluetoothBLEHID.send(report, sizeof(report));

  delay(8);

//   uint8_t release[9] = {1, 0, 0, 0, 0, 0, 0, 0, 0};
//   PicoBluetoothBLEHID.send(release, sizeof(release));
}

#endif  // PICO_CYW43_SUPPORTED