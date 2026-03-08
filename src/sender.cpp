#include "sender.h"

using namespace uwu;

#if IS_MCU_VERSION != 0

uint8_t desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD)),
    TUD_HID_REPORT_DESC_GAMEPAD (HID_REPORT_ID(RID_GAMEPAD)),
    TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(RID_CONSUMER_CONTROL))
};

Adafruit_USBD_HID usb_hid((const uint8_t *)desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

#endif

static bool bluetooth_mode = false;
static keycode_node single_keycode_node[NUM_REPORT_IDs];
static keycode_node empty_keycode_node = {};

void uwu::init_sender(int bluetooth_timeout)
{
#if IS_MCU_VERSION != 0
    usb_hid.begin();

    // Wait for a bit for USB
  while (!TinyUSBDevice.mounted() && bluetooth_timeout > 0) {
    delay(100); // NOLINT(readability-magic-numbers)
    bluetooth_timeout -= 100; // NOLINT(readability-magic-numbers)
  }

  // if USB timeout, switch to bluetooth mode
  if (bluetooth_timeout <= 0) {
    bluetooth_mode = true;
  }
#else
    printf("init_sender\n");
#endif
}

void uwu::send_kecycode_keyboard(keycode_node* node)
{
    if (!bluetooth_mode)
    {
#if IS_MCU_VERSION != 0
        while (!usb_hid.ready())
            delayMicroseconds(1);
        usb_hid.keyboardReport(node->r_id, node->mod, node->codes);
#else
        printf("sending keyboard ");
        print_node(node);
#endif
    }
    else
    {
        // TODO: Handle BLE
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
    }

}

void uwu::send_keycode_single(keycode_node* root)
{
    int free_pos = -1;
    int r_id = root->r_id;
    single_keycode_node[r_id].r_id = root->r_id;    // redundant, but hey...
    for(int i = 0; i < 6; i++)
        if(single_keycode_node[r_id].codes[i] == '\0')
        {
            free_pos = i;
            break;
        }

    if(free_pos > -1)
    {
        printf("\tSingle 1\n");
        single_keycode_node[r_id].codes[free_pos] = root->codes[0];
    }
    single_keycode_node[r_id].mod |= root->mod;
    switch (r_id)
    {
        case RID_KEYBOARD:
            send_kecycode_keyboard(&single_keycode_node[r_id]);
            break;
    }
}


void uwu::send_keycode(keycode_node* root, bool is_single)
{
    if (root == nullptr)
        return;

    if(is_single)
    {
        send_keycode_single(root);
    }
    else
    {
        keycode_node* node = root;
        int i = 0;
        do
        {
            send_keycode_node(node);
            node = node->next;
            i++;
        }while (node != nullptr && i < KEYCODE_MEM_SIZE);
    }

}

void uwu::clear_keycode(keycode_node* root, bool is_single)
{
    int r_id = root->r_id;

    if(is_single)
    {
        for(int i = 0; i < 6; i++)
            if(single_keycode_node[r_id].codes[i] == '\0')
                single_keycode_node[r_id].codes[i] = '\0';
        single_keycode_node[r_id].mod &= ~(root->mod);
    }
    else
    {
        send_keycode_node(&empty_keycode_node);
    }
}