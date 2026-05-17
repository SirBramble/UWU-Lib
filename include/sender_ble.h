#ifndef SENDER_BLE_H
#define SENDER_BLE_H

#include "memory.h"

void sender_ble_begin();

bool sender_ble_running();

void sendBleKeyboardReport(uwu::keycode_node* node);

#endif // SENDER_BLE_H