#ifndef SENDER_H
#define SENDER_H

#include "memory.h"
#include "report_id.h"

#if IS_MCU_VERSION != 0
    #include <Arduino.h>
    #include <Adafruit_TinyUSB.h>
    #include <MIDI.h>
#endif

namespace uwu
{

void init_sender(int bluetooth_timeout);

void send_kecycode_keyboard(keycode_node* node);

void send_keycode_node(keycode_node* node);

void send_keycode_single(keycode_node* root);

void send_keycode(keycode_node* root, bool is_single);

void clear_keycode(keycode_node* root, bool is_single);

}

#endif // SENDER_H
