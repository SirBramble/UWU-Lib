#include "sender.h"
#include "report_id.h"

using namespace uwu;

#if PICO_CYW43_SUPPORTED == 1
#include "sender_ble.h"
#endif

#if IS_MCU_VERSION == 0
    #include <cstdlib>
    #include <stdio.h>
    #define PRINT(...) printf(__VA_ARGS__)
#else
    #include <Arduino.h>
    #define PRINT(...) Serial.printf(__VA_ARGS__)
#endif

#if IS_MCU_VERSION != 0

uint8_t desc_hid_report[] = {
    TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(RID_GAMEPAD)),
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD)),
    TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(RID_CONSUMER_CONTROL))
};

Adafruit_USBD_HID usb_hid((const uint8_t *)desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);
// Adafruit_USBD_MIDI usb_midi;
// MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

#endif

static bool bluetooth_mode = false;
static keycode_node single_keycode_node[NUM_REPORT_IDs];
static keycode_node empty_keycode_node = {{0,0,0,0,0,0},0,RID_KEYBOARD,false,nullptr};

void uwu::init_sender(int bluetooth_timeout)
{
#if IS_MCU_VERSION != 0
    if (!TinyUSBDevice.isInitialized()) {
        TinyUSBDevice.begin(0);
    }

    usb_hid.setPollInterval(2);
    usb_hid.setStringDescriptor("UWU HID");
    usb_hid.begin();

    // TinyUSBDevice.detach();
    // delay(1000);
    // TinyUSBDevice.attach();

//     while (!TinyUSBDevice.mounted() && bluetooth_timeout > 0) {
// #ifdef TINYUSB_NEED_POLLING_TASK
//         TinyUSBDevice.task();
// #endif
//         delay(10);
//         bluetooth_timeout -= 10;
//     }

//     if (bluetooth_timeout <= 0) {
//         bluetooth_mode = true;
//     }

    PRINT("mounted=%d valid=%d ready=%d\n", TinyUSBDevice.mounted(), usb_hid.isValid(), usb_hid.ready());

#if PICO_CYW43_SUPPORTED == 1
    // TODO: Check for bluetooth mode or something
    sender_ble_begin();
#endif

#else
    printf("init_sender\n");
#endif
}

void uwu::send_kecycode_keyboard(keycode_node* node)
{
    if (!bluetooth_mode)
    {
#if IS_MCU_VERSION != 0
    if (!TinyUSBDevice.mounted()) {
        PRINT("USB not mounted\n");
        return;
    }

    uint32_t start = millis();
    while (!usb_hid.ready()) {
#ifdef TINYUSB_NEED_POLLING_TASK
        TinyUSBDevice.task();
#endif
        delay(1);

        if (millis() - start > 100) {
            PRINT("HID timeout m=%d v=%d r=%d\n", TinyUSBDevice.mounted(), usb_hid.isValid(), usb_hid.ready());
            return;
        }
    }

    bool ok = usb_hid.keyboardReport(node->r_id, node->mod, node->codes);
    PRINT("send ok=%d rid=%u mod=%02X codes=%02X, %02X, %02X, %02X, %02X, %02X\n",
          ok, node->r_id, node->mod, node->codes[0], node->codes[1], node->codes[2], node->codes[3], node->codes[4], node->codes[5]);
#else
    printf("sending keyboard ");
    print_node(node);
#endif
    }
    else
    {
        if(sender_ble_running())
            sendBleKeyboardReport(node);
    }

}

void uwu::send_keycode_node(keycode_node* node)
{
    if (node == nullptr)
        return;
    switch (node->r_id)
    {
        case RID_KEYBOARD:
        send_kecycode_keyboard(node);
            break;
        default:
        PRINT("RID fucked!: (%d)", node->r_id);
            break;
    }

}

void uwu::send_keycode_single(keycode_node* root)
{
    int free_pos = -1;
    int r_id = root->r_id;

    if((uint)r_id >= NUM_REPORT_IDs)
        return;

    single_keycode_node[r_id].r_id = root->r_id;    // nedded as initial rid for single is -1
    for(int i = 0; i < 6; i++)
        if(single_keycode_node[r_id].codes[i] == '\0')
        {
            free_pos = i;
            break;
        }

    if(free_pos > -1)
    {
        PRINT("\tSingle 1\n");
        single_keycode_node[r_id].codes[free_pos] = root->codes[0];
    }
    single_keycode_node[r_id].mod |= root->mod;
    Serial.print(r_id);Serial.printf(" %d ",root->next);
    switch (r_id)
    {
        case RID_KEYBOARD:
            send_kecycode_keyboard(&single_keycode_node[r_id]);
            break;
        default:
            break;
    }
}


void uwu::send_keycode(keycode_node* root, bool is_single)
{
    if (root == nullptr)
    {
        return;
    }
    
    if(is_single)
    {
        send_keycode_single(root);
        Serial.printf("sent single");
    }
    else
    {
        keycode_node* node = root;
        int i = 0;
        do
        {
            send_keycode_node(node);
            Serial.printf("sent cluster");
            node = node->next;
            i++;
        }while (node != nullptr && i < KEYCODE_MEM_SIZE);
    }

}

void uwu::clear_keycode(keycode_node* root, bool is_single)
{
    if (root == nullptr) {
        PRINT("clear_keycode root=null\n");
        return;
    }

    int r_id = root->r_id;  // ROOT NEVER INITIALISED OR SOMETHING

    Serial.printf("r_id: %d\tnext: %p\tcodes[0]: %d\tcodes[1]: %d\tcodes[2]: %d\tcodes[3]: %d\tcodes[4]: %d\tcodes[5]: %d\t\n", r_id, root->next, root->codes[0], root->codes[1], root->codes[2], root->codes[3], root->codes[4], root->codes[5]);

    if(is_single)
    {
         for (int i = 0; i < 6; i++)
        {
            if (single_keycode_node[r_id].codes[i] == root->codes[0])
                single_keycode_node[r_id].codes[i] = 0;
        }

        single_keycode_node[r_id].mod &= ~(root->mod);
        send_keycode_node(&single_keycode_node[r_id]);
    }
    else
    {
        send_keycode_node(&empty_keycode_node);
    }
}

void uwu::sender_loop()
{
#ifdef TINYUSB_NEED_POLLING_TASK
    TinyUSBDevice.task();
#endif
}