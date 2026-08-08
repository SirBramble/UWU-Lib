#include "animacro.h"
#include "class/hid/hid.h"
#include "color.h"
#include "memory.h"
#include "report_id.h"



#if IS_MCU_VERSION == 0
    #include <cstdlib>
    #include <stdio.h>
    #define PRINT(...) printf(__VA_ARGS__)
#else
    #include <Arduino.h>
    #define PRINT(...) Serial.printf(__VA_ARGS__)
#endif

using namespace uwu;


// #include <string.h>

// const char* s = "hello world";
// char* p = strstr(s, "world");

// if (p) {
//     // p points to "world"
// }

// Main SM loop
bool animacro_parser::am_data_parse(const char* c_str, key* k)
{
    PRINT("data_parse: %s\n", c_str);
    if(c_str == nullptr)
        return false;
    if(c_str[0] == '\0')  // If empty String is passed...Counts more as DONE than ERROR...
        return true;
    if(k == nullptr)
        return false;

    m_am_pos = 0;
    m_am_str = c_str;
    m_am_key = k;
    m_am_state = am_state_t::IDLE;

    for(int i = 0; i < AM_MAX_LOOP; i++)
    {

        // By default, move to the next char with every loop iteration. In Case of leaf parser nodes (e.g. COMMAND_STRG, COMMAND_NULL, etc.)
        // that ignore the current char, rerun the loop without moving to the next char.
        bool next_char = true;

        switch (m_am_state)
        {
        case am_state_t::IDLE:
            PRINT("IDLE: %c\n", m_am_str[m_am_pos]);
            if(m_am_str[m_am_pos] == '\0')
                break;

            if(isprint((unsigned char)m_am_str[m_am_pos]) && m_am_str[m_am_pos] != '\\')
            {
                append_keycode(m_am_str[m_am_pos]);
                break;
            }
            else if(m_am_str[m_am_pos] == '\\')
            {
                m_am_state = am_state_t::COMMAND;
                break;
            }
            break;
        case am_state_t::COMMAND:
            PRINT("COMMAND: %c\n", m_am_str[m_am_pos]);
            switch (m_am_str[m_am_pos])
            {
                case 'A':
                    m_am_state = COMMAND_A;
                    break;
                case 'B':
                    m_am_state = COMMAND_B;
                    break;
                case 'D':
                    m_am_state = COMMAND_D;
                    break;
                case 'E':
                    m_am_state = COMMAND_E;
                    break;
                case 'F':
                    m_am_state = COMMAND_F;
                    break;
                case 'K':
                    m_am_state = COMMAND_K;
                    break;
                case 'L':
                    m_am_state = COMMAND_L;
                    break;
                case 'N':
                    m_am_state = COMMAND_N;
                    break;
                case 'O':
                    m_am_state = COMMAND_O;
                    break;
                case 'R':
                    m_am_state = COMMAND_R;
                    break;
                case 'S':
                    m_am_state = COMMAND_S;
                    break;
                case 'T':
                    m_am_state = COMMAND_T;
                    break;
                case 'U':
                    m_am_state = COMMAND_U;
                    break;
                case '\\':
                    m_am_state = COMMAND_BACKSLASH;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::DONE:
            PRINT("DONE\n");
            return true;
            break;
        case am_state_t::ERROR:
            PRINT("ERROR\n");
            return false;
            break;
        // *************** COMMAND PARSE *************** //
        // COMMAND BACKSLASH
        case am_state_t::COMMAND_BACKSLASH:
            PRINT("COMMAND_BACKSLASH\n");
            {
                bool success = append_keycode('\\');
                m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                next_char = false;
            }
            break;
        // Command AE
        case am_state_t::COMMAND_A:
			PRINT("COMMAND_A\n");
			switch (m_am_str[m_am_pos])
			{
				case 'E':
					m_am_state = COMMAND_AE;
					break;
				default:
					m_am_state = ERROR;
					break;
			}
			break;
		case am_state_t::COMMAND_AE:
			PRINT("COMMAND_AE\n");
			switch (m_am_str[m_am_pos])
			{
				default:
					bool success = append_keycode(0x34,0,RID_KEYBOARD);
					m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
					next_char = false;
					break;
			}
			break;

        // COMMAND BACKSPACE
        case am_state_t::COMMAND_B:
            PRINT("COMMAND_B\n");
            switch (m_am_str[m_am_pos])
            {
                case 'A':
                    m_am_state = COMMAND_BA;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_BA:
            PRINT("COMMAND_BA\n");
            switch (m_am_str[m_am_pos])
            {
                case 'C':
                    m_am_state = COMMAND_BAC;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_BAC:
            PRINT("COMMAND_BAC\n");
            switch (m_am_str[m_am_pos])
            {
                case 'K':
                    m_am_state = COMMAND_BACK;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_BACK:
            PRINT("COMMAND_BACK\n");
            switch (m_am_str[m_am_pos])
            {
                case 'S':
                    m_am_state = COMMAND_BACKS;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_BACKS:
            PRINT("COMMAND_BACKS\n");
            switch (m_am_str[m_am_pos])
            {
                case 'P':
                    m_am_state = COMMAND_BACKSP;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_BACKSP:
            PRINT("COMMAND_BACKSP\n");
            switch (m_am_str[m_am_pos])
            {
                case 'A':
                    m_am_state = COMMAND_BACKSPA;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_BACKSPA:
            PRINT("COMMAND_B\n");
            switch (m_am_str[m_am_pos])
            {
                case 'C':
                    m_am_state = COMMAND_BACKSPAC;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_BACKSPAC:
            PRINT("COMMAND_BACKSPAC\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_BACKSPACE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        // COMMAND BACKSPACE
        case am_state_t::COMMAND_BACKSPACE:
            PRINT("COMMAND_BACKSPACE\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_BACKSPACE,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // COMMAND DOWN_KEY/DELAY
        case am_state_t::COMMAND_D:
            PRINT("COMMAND_D\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_DE;
                    break;
                case 'O':
                    m_am_state = COMMAND_DO;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DE:
            PRINT("COMMAND_DE\n");
            switch (m_am_str[m_am_pos])
            {
                case 'L':
                    m_am_state = COMMAND_DEL;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DEL:
            PRINT("COMMAND_DEL\n");
            switch (m_am_str[m_am_pos])
            {
                case 'A':
                    m_am_state = COMMAND_DELA;
                    break;
                case 'E':
                    m_am_state = COMMAND_DELE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DELA:
            PRINT("COMMAND_DELA\n");
            switch (m_am_str[m_am_pos])
            {
                case 'Y':
                    m_am_state = COMMAND_DELAY;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DELAY:
            PRINT("COMMAND_DELAY\n");
            switch (m_am_str[m_am_pos])
            {
                case '{':
                    m_num_1 = 0;
                    m_am_state = COMMAND_DELAY_BR_OPEN;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DELAY_BR_OPEN:
            PRINT("COMMAND_DELAY_BR_OPEN\n");
            if(m_am_str[m_am_pos] >= '0' && m_am_str[m_am_pos] <= '9')
            {
                m_num_1 *= 10;
                m_num_1 += m_am_str[m_am_pos] - '0';
            }
            else
            {
                m_am_state = COMMAND_DELAY_BR_CLOSE;
            }
            break;
        case am_state_t::COMMAND_DELAY_BR_CLOSE:
            PRINT("COMMAND_DELAY_BR_CLOSE\n");
            m_am_state = append_delay_node(m_num_1*1000) ? am_state_t::IDLE : am_state_t::ERROR;
            next_char = false;
            break;
        case am_state_t::COMMAND_DO:
            PRINT("COMMAND_DO\n");
            switch (m_am_str[m_am_pos])
            {
                case 'W':
                    m_am_state = COMMAND_DOW;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DELE:
            PRINT("COMMAND_DELE\n");
            switch (m_am_str[m_am_pos])
            {
                case 'T':
                    m_am_state = COMMAND_DELET;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DELET:
            PRINT("COMMAND_DELET\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_DELETE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DELETE:
            PRINT("COMMAND_DELETE\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x4C,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        case am_state_t::COMMAND_DOW:
            PRINT("COMMAND_DOW\n");
            switch (m_am_str[m_am_pos])
            {
                case 'N':
                    m_am_state = COMMAND_DOWN;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DOWN:
            PRINT("COMMAND_DOWN\n");
            switch (m_am_str[m_am_pos])
            {
                case '_':
                    m_am_state = COMMAND_DOWN_;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DOWN_:
            PRINT("COMMAND_DOWN_\n");
            switch (m_am_str[m_am_pos])
            {
                case 'K':
                    m_am_state = COMMAND_DOWN_K;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DOWN_K:
            PRINT("COMMAND_DOWN_K\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_DOWN_KE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DOWN_KE:
            PRINT("COMMAND_DOWN_KE\n");
            switch (m_am_str[m_am_pos])
            {
                case 'Y':
                    m_am_state = COMMAND_DOWN_KEY;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_DOWN_KEY:
            PRINT("COMMAND_DOWN_KEY\n");
            switch (m_am_str[m_am_pos])
            {
                case '{':
                    m_num_1 = 0;
                    m_am_state = COMMAND_DOWN_KEY_BR_OPEN;
                    break;
                default:
                    bool success = append_keycode(HID_KEY_ARROW_DOWN,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        case am_state_t::COMMAND_DOWN_KEY_BR_OPEN:
            PRINT("COMMAND_DOWN_KEY_BR_OPEN\n");
            if(m_am_str[m_am_pos] >= '0' && m_am_str[m_am_pos] <= '9')
            {
                m_num_1 *= 10;
                m_num_1 += m_am_str[m_am_pos] - '0';
            }
            else
            {
                m_am_state = COMMAND_DOWN_KEY_BR_CLOSE;
            }
            break;
        case am_state_t::COMMAND_DOWN_KEY_BR_CLOSE:
            PRINT("COMMAND_DOWN_KEY_BR_CLOSE\n");
            for(int i = 0; i < m_num_1; i++)
            {
                bool success = append_keycode(HID_KEY_ARROW_DOWN,0,RID_KEYBOARD);
                m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
            }
            next_char = false;
            break;

        // COMMAND ESC/ENTER
        case am_state_t::COMMAND_E:
            PRINT("COMMAND_E\n");
            switch (m_am_str[m_am_pos])
            {
                case 'S':
                    m_am_state = COMMAND_ES;
                    break;
                case 'N':
                    m_am_state = COMMAND_EN;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        
        // COMMAND ESC
        case am_state_t::COMMAND_ES:
            PRINT("COMMAND_ES\n");
            switch (m_am_str[m_am_pos])
            {
                case 'C':
                    m_am_state = COMMAND_ESC;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_ESC:
            PRINT("COMMAND_ESC\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_ESCAPE,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // COMMAND ENTER
        case am_state_t::COMMAND_EN:
            PRINT("COMMAND_EN\n");
            switch (m_am_str[m_am_pos])
            {
                case 'T':
                    m_am_state = COMMAND_ENT;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_ENT:
            PRINT("COMMAND_ENT\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_ENTE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_ENTE:
            PRINT("COMMAND_ENTE\n");
            switch (m_am_str[m_am_pos])
            {
                case 'R':
                    m_am_state = COMMAND_ENTER;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_ENTER:
            PRINT("COMMAND_ENTER\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_ENTER,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // COMMAND F1-F24
        case am_state_t::COMMAND_F:
            PRINT("COMMAND_F\n");
            switch (m_am_str[m_am_pos])
            {
                case '1':
                    m_am_state = COMMAND_F1;
                    break;
                case '2':
                    m_am_state = COMMAND_F2;
                    break;
                case '3':
                    m_am_state = COMMAND_F3;
                    break;
                case '4':
                    m_am_state = COMMAND_F4;
                    break;
                case '5':
                    m_am_state = COMMAND_F5;
                    break;
                case '6':
                    m_am_state = COMMAND_F6;
                    break;
                case '7':
                    m_am_state = COMMAND_F7;
                    break;
                case '8':
                    m_am_state = COMMAND_F8;
                    break;
                case '9':
                    m_am_state = COMMAND_F9;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        // Command F1 & F10-F19
        case am_state_t::COMMAND_F1:
            PRINT("COMMAND_F1\n");
            switch (m_am_str[m_am_pos])
            {
                case '1':
                    m_am_state = COMMAND_F11;
                    break;
                case '2':
                    m_am_state = COMMAND_F12;
                    break;
                case '3':
                    m_am_state = COMMAND_F13;
                    break;
                case '4':
                    m_am_state = COMMAND_F14;
                    break;
                case '5':
                    m_am_state = COMMAND_F15;
                    break;
                case '6':
                    m_am_state = COMMAND_F16;
                    break;
                case '7':
                    m_am_state = COMMAND_F17;
                    break;
                case '8':
                    m_am_state = COMMAND_F18;
                    break;
                case '9':
                    m_am_state = COMMAND_F19;
                    break;
                default:
                    bool success = append_keycode(HID_KEY_F1,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;

        // Command F2 & F20-F24
        case am_state_t::COMMAND_F2:
            PRINT("COMMAND_F2\n");
            switch (m_am_str[m_am_pos])
            {
                case '1':
                    m_am_state = COMMAND_F21;
                    break;
                case '2':
                    m_am_state = COMMAND_F22;
                    break;
                case '3':
                    m_am_state = COMMAND_F23;
                    break;
                case '4':
                    m_am_state = COMMAND_F24;
                    break;
                default:
                    bool success = append_keycode(HID_KEY_F2,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;

        // Command F3
        case am_state_t::COMMAND_F3:
            PRINT("COMMAND_F3\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F3,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F4
        case am_state_t::COMMAND_F4:
            PRINT("COMMAND_F4\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F4,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F5
        case am_state_t::COMMAND_F5:
            PRINT("COMMAND_F5\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F5,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F6
        case am_state_t::COMMAND_F6:
            PRINT("COMMAND_F6\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F6,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F7
        case am_state_t::COMMAND_F7:
            PRINT("COMMAND_F7\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F7,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F8
        case am_state_t::COMMAND_F8:
            PRINT("COMMAND_F8\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F8,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F9
        case am_state_t::COMMAND_F9:
            PRINT("COMMAND_F9\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F9,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F10
        case am_state_t::COMMAND_F10:
            PRINT("COMMAND_F10\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F10,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F11
        case am_state_t::COMMAND_F11:
            PRINT("COMMAND_F11\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F11,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F12
        case am_state_t::COMMAND_F12:
            PRINT("COMMAND_F12\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F12,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F13
        case am_state_t::COMMAND_F13:
            PRINT("COMMAND_F13\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F13,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F14
        case am_state_t::COMMAND_F14:
            PRINT("COMMAND_F14\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F14,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F115
        case am_state_t::COMMAND_F15:
            PRINT("COMMAND_F15\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F15,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F16
        case am_state_t::COMMAND_F16:
            PRINT("COMMAND_F16\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F16,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F17
        case am_state_t::COMMAND_F17:
            PRINT("COMMAND_F17\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F17,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F18
        case am_state_t::COMMAND_F18:
            PRINT("COMMAND_F18\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F18,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F19
        case am_state_t::COMMAND_F19:
            PRINT("COMMAND_F19\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F19,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F20
        case am_state_t::COMMAND_F20:
            PRINT("COMMAND_F20\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F20,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F21
        case am_state_t::COMMAND_F21:
            PRINT("COMMAND_F21\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F21,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F22
        case am_state_t::COMMAND_F22:
            PRINT("COMMAND_F22\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F22,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F23
        case am_state_t::COMMAND_F23:
            PRINT("COMMAND_F23\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F23,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // Command F24
        case am_state_t::COMMAND_F24:
            PRINT("COMMAND_F24\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(HID_KEY_F24,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // COMMAND arbitrary KEY
        case am_state_t::COMMAND_K:
            PRINT("COMMAND_K\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_KE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_KE:
            PRINT("COMMAND_KE\n");
            switch (m_am_str[m_am_pos])
            {
                case 'Y':
                    m_am_state = COMMAND_KEY;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        
        case am_state_t::COMMAND_KEY:
            PRINT("COMMAND_KEY\n");
            switch (m_am_str[m_am_pos])
            {
                case '{':
                    m_am_state = COMMAND_KEY_BR_OPEN;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
            
        case am_state_t::COMMAND_KEY_BR_OPEN:
            PRINT("COMMAND_KEY_BR_OPEN\n");
            m_num_1 = 0;
            m_num_2 = 0;
            switch (m_am_str[m_am_pos])
            {
                case '0':
                    // Do nothing
                    break;
                case 'h':
                    m_am_state = COMMAND_KEY_BR_OPEN_HEX;
                    break;
                case 'H':
                    m_am_state = COMMAND_KEY_BR_OPEN_HEX;
                    break;
                case 'x':
                    m_am_state = COMMAND_KEY_BR_OPEN_HEX;
                    break;
                case 'X':
                    m_am_state = COMMAND_KEY_BR_OPEN_HEX;
                    break;
                case '1':
                    m_am_state = COMMAND_KEY_BR_OPEN_DEC;
                    next_char = false;
                    break;
                case '2':
                    m_am_state = COMMAND_KEY_BR_OPEN_DEC;
                    next_char = false;
                    break;
                case '3':
                    m_am_state = COMMAND_KEY_BR_OPEN_DEC;
                    next_char = false;
                    break;
                case '4':
                    m_am_state = COMMAND_KEY_BR_OPEN_DEC;
                    next_char = false;
                    break;
                case '5':
                    m_am_state = COMMAND_KEY_BR_OPEN_DEC;
                    next_char = false;
                    break;
                case '6':
                    m_am_state = COMMAND_KEY_BR_OPEN_DEC;
                    next_char = false;
                    break;
                case '7':
                    m_am_state = COMMAND_KEY_BR_OPEN_DEC;
                    next_char = false;
                    break;
                case '8':
                    m_am_state = COMMAND_KEY_BR_OPEN_DEC;
                    next_char = false;
                    break;
                case '9':
                    m_am_state = COMMAND_KEY_BR_OPEN_DEC;
                    next_char = false;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_KEY_BR_OPEN_HEX:
            PRINT("COMMAND_KEY_BR_OPEN_HEX\n");
            switch (m_am_str[m_am_pos])
            {
                case '0':
                    m_num_1 = (m_num_1 * 16) + 0;
                    break;
                case '1':
                    m_num_1 = (m_num_1 * 16) + 1;
                    break;
                case '2':
                    m_num_1 = (m_num_1 * 16) + 2;
                    break;
                case '3':
                    m_num_1 = (m_num_1 * 16) + 3;
                    break;
                case '4':
                    m_num_1 = (m_num_1 * 16) + 4;
                    break;
                case '5':
                    m_num_1 = (m_num_1 * 16) + 5;
                    break;
                case '6':
                    m_num_1 = (m_num_1 * 16) + 6;
                    break;
                case '7':
                    m_num_1 = (m_num_1 * 16) + 7;
                    break;
                case '8':
                    m_num_1 = (m_num_1 * 16) + 8;
                    break;
                case '9':
                    m_num_1 = (m_num_1 * 16) + 9;
                    break;
                case 'a':
                    m_num_1 = (m_num_1 * 16) + 10;
                    break;
                case 'A':
                    m_num_1 = (m_num_1 * 16) + 10;
                    break;
                case 'b':
                    m_num_1 = (m_num_1 * 16) + 11;
                    break;
                case 'B':
                    m_num_1 = (m_num_1 * 16) + 11;
                    break;
                case 'c':
                    m_num_1 = (m_num_1 * 16) + 12;
                    break;
                case 'C':
                    m_num_1 = (m_num_1 * 16) + 12;
                    break;
                case 'd':
                    m_num_1 = (m_num_1 * 16) + 13;
                    break;
                case 'D':
                    m_num_1 = (m_num_1 * 16) + 13;
                    break;
                case 'e':
                    m_num_1 = (m_num_1 * 16) + 14;
                    break;
                case 'E':
                    m_num_1 = (m_num_1 * 16) + 14;
                    break;
                case 'f':
                    m_num_1 = (m_num_1 * 16) + 15;
                    break;
                case 'F':
                    m_num_1 = (m_num_1 * 16) + 15;
                    break;
                case 'm':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD;
                    break;
                case 'M':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD;
                    break;
                case ',':
                    m_am_state = COMMAND_KEY_BR_OPEN_NEXT;
                    break;
                case '}':
                    m_am_state = COMMAND_KEY_BR_CLOSE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_KEY_BR_OPEN_DEC:
            PRINT("COMMAND_KEY_BR_OPEN_DEC\n");
            switch (m_am_str[m_am_pos])
            {
                case '0':
                    m_num_1 = (m_num_1 * 10) + 0;
                    break;
                case '1':
                    m_num_1 = (m_num_1 * 10) + 1;
                    break;
                case '2':
                    m_num_1 = (m_num_1 * 10) + 2;
                    break;
                case '3':
                    m_num_1 = (m_num_1 * 10) + 3;
                    break;
                case '4':
                    m_num_1 = (m_num_1 * 10) + 4;
                    break;
                case '5':
                    m_num_1 = (m_num_1 * 10) + 5;
                    break;
                case '6':
                    m_num_1 = (m_num_1 * 10) + 6;
                    break;
                case '7':
                    m_num_1 = (m_num_1 * 10) + 7;
                    break;
                case '8':
                    m_num_1 = (m_num_1 * 10) + 8;
                    break;
                case '9':
                    m_num_1 = (m_num_1 * 10) + 9;
                    break;
                case 'm':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD;
                    break;
                case 'M':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD;
                    break;
                case ',':
                    m_am_state = COMMAND_KEY_BR_OPEN_NEXT;
                    break;
                case '}':
                    m_am_state = COMMAND_KEY_BR_CLOSE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_KEY_BR_OPEN_MOD:
            PRINT("COMMAND_KEY_BR_OPEN_MOD\n");
            m_num_2 = 0;
            switch (m_am_str[m_am_pos])
            {
                case '0':
                    // Do nothing
                    break;
                case 'h':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_HEX;
                    break;
                case 'H':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_HEX;
                    break;
                case 'x':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_HEX;
                    break;
                case 'X':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_HEX;
                    break;
                case '1':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_DEC;
                    next_char = false;
                    break;
                case '2':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_DEC;
                    next_char = false;
                    break;
                case '3':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_DEC;
                    next_char = false;
                    break;
                case '4':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_DEC;
                    next_char = false;
                    break;
                case '5':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_DEC;
                    next_char = false;
                    break;
                case '6':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_DEC;
                    next_char = false;
                    break;
                case '7':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_DEC;
                    next_char = false;
                    break;
                case '8':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_DEC;
                    next_char = false;
                    break;
                case '9':
                    m_am_state = COMMAND_KEY_BR_OPEN_MOD_DEC;
                    next_char = false;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        
        case am_state_t::COMMAND_KEY_BR_OPEN_MOD_HEX:
            PRINT("COMMAND_KEY_BR_OPEN_MOD_HEX\n");
            switch (m_am_str[m_am_pos])
            {
                case '0':
                    m_num_2 = (m_num_2 * 16) + 0;
                    break;
                case '1':
                    m_num_2 = (m_num_2 * 16) + 1;
                    break;
                case '2':
                    m_num_2 = (m_num_2 * 16) + 2;
                    break;
                case '3':
                    m_num_2 = (m_num_2 * 16) + 3;
                    break;
                case '4':
                    m_num_2 = (m_num_2 * 16) + 4;
                    break;
                case '5':
                    m_num_2 = (m_num_2 * 16) + 5;
                    break;
                case '6':
                    m_num_2 = (m_num_2 * 16) + 6;
                    break;
                case '7':
                    m_num_2 = (m_num_2 * 16) + 7;
                    break;
                case '8':
                    m_num_2 = (m_num_2 * 16) + 8;
                    break;
                case '9':
                    m_num_2 = (m_num_2 * 16) + 9;
                    break;
                case 'a':
                    m_num_2 = (m_num_2 * 16) + 10;
                    break;
                case 'A':
                    m_num_2 = (m_num_2 * 16) + 10;
                    break;
                case 'b':
                    m_num_2 = (m_num_2 * 16) + 11;
                    break;
                case 'B':
                    m_num_2 = (m_num_2 * 16) + 11;
                    break;
                case 'c':
                    m_num_2 = (m_num_2 * 16) + 12;
                    break;
                case 'C':
                    m_num_2 = (m_num_2 * 16) + 12;
                    break;
                case 'd':
                    m_num_2 = (m_num_2 * 16) + 13;
                    break;
                case 'D':
                    m_num_2 = (m_num_2 * 16) + 13;
                    break;
                case 'e':
                    m_num_2 = (m_num_2 * 16) + 14;
                    break;
                case 'E':
                    m_num_2 = (m_num_2 * 16) + 14;
                    break;
                case 'f':
                    m_num_2 = (m_num_2 * 16) + 15;
                    break;
                case 'F':
                    m_num_2 = (m_num_2 * 16) + 15;
                    break;
                case ',':
                    m_am_state = COMMAND_KEY_BR_OPEN_NEXT;
                    break;
                case '}':
                    m_am_state = COMMAND_KEY_BR_CLOSE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_KEY_BR_OPEN_MOD_DEC:
            PRINT("COMMAND_KEY_BR_OPEN_MOD_DEC\n");
            switch (m_am_str[m_am_pos])
            {
                case '0':
                    m_num_2 = (m_num_2 * 10) + 0;
                    break;
                case '1':
                    m_num_2 = (m_num_2 * 10) + 1;
                    break;
                case '2':
                    m_num_2 = (m_num_2 * 10) + 2;
                    break;
                case '3':
                    m_num_2 = (m_num_2 * 10) + 3;
                    break;
                case '4':
                    m_num_2 = (m_num_2 * 10) + 4;
                    break;
                case '5':
                    m_num_2 = (m_num_2 * 10) + 5;
                    break;
                case '6':
                    m_num_2 = (m_num_2 * 10) + 6;
                    break;
                case '7':
                    m_num_2 = (m_num_2 * 10) + 7;
                    break;
                case '8':
                    m_num_2 = (m_num_2 * 10) + 8;
                    break;
                case '9':
                    m_num_2 = (m_num_2 * 10) + 9;
                    break;
                case ',':
                    m_am_state = COMMAND_KEY_BR_OPEN_NEXT;
                    break;
                case '}':
                    m_am_state = COMMAND_KEY_BR_CLOSE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_KEY_BR_OPEN_NEXT:
            PRINT("COMMAND_KEY_BR_OPEN_NEXT\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(m_num_1,m_num_2,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::COMMAND_KEY_BR_OPEN : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        case am_state_t::COMMAND_KEY_BR_CLOSE:
            PRINT("COMMAND_KEY_BR_CLOSE\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(m_num_1,m_num_2,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;

        // COMMAND L_ALT/L_KEY/L_SHIFT/L_STRG/L_META
        case am_state_t::COMMAND_L:
            PRINT("COMMAND_L\n");
            switch (m_am_str[m_am_pos])
            {
                case '_':
                    m_am_state = COMMAND_L_;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_:
            PRINT("COMMAND_L_\n");
            switch (m_am_str[m_am_pos])
            {
                case 'A':
                    m_am_state = COMMAND_L_A;
                    break;
                case 'K':
                    m_am_state = COMMAND_L_K;
                    break;
                case 'M':
                    m_am_state = COMMAND_L_M;
                    break;
                case 'S':
                    m_am_state = COMMAND_L_S;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_A:
            PRINT("COMMAND_L_A\n");
            switch (m_am_str[m_am_pos])
            {
                case 'L':
                    m_am_state = COMMAND_L_AL;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_AL:
            PRINT("COMMAND_L_AL\n");
            switch (m_am_str[m_am_pos])
            {
                case 'T':
                    m_am_state = COMMAND_L_ALT;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_ALT:
            PRINT("COMMAND_L_ALT\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x00,KEY_MOD_LALT,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_K:
            PRINT("COMMAND_L_K\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_L_KE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_KE:
            PRINT("COMMAND_L_KE\n");
            switch (m_am_str[m_am_pos])
            {
                case 'Y':
                    m_am_state = COMMAND_L_KEY;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_KEY:
            PRINT("COMMAND_L_KEY\n");
            switch (m_am_str[m_am_pos])
            {
                case '{':
                    m_num_1 = 0;
                    m_am_state = COMMAND_L_KEY_BR_OPEN;
                    break;
                default:
                    bool success = append_keycode(HID_KEY_ARROW_LEFT,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_KEY_BR_OPEN:
            PRINT("COMMAND_L_KEY_BR_OPEN\n");
            if(m_am_str[m_am_pos] >= '0' && m_am_str[m_am_pos] <= '9')
            {
                m_num_1 *= 10;
                m_num_1 += m_am_str[m_am_pos] - '0';
            }
            else
            {
                m_am_state = COMMAND_L_KEY_BR_CLOSE;
            }
            break;
        case am_state_t::COMMAND_L_KEY_BR_CLOSE:
            PRINT("COMMAND_L_KEY_BR_CLOSE\n");
            for(int i = 0; i < m_num_1; i++)
            {
                bool success = append_keycode(HID_KEY_ARROW_LEFT,0,RID_KEYBOARD);
                m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
            }
            next_char = false;
            break;
        
        case am_state_t::COMMAND_L_M:
            PRINT("COMMAND_L_M\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_L_ME;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_ME:
            PRINT("COMMAND_L_ME\n");
            switch (m_am_str[m_am_pos])
            {
                case 'T':
                    m_am_state = COMMAND_L_MET;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_MET:
            PRINT("COMMAND_L_MET\n");
            switch (m_am_str[m_am_pos])
            {
                case 'A':
                    m_am_state = COMMAND_L_META;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_META:
            PRINT("COMMAND_L_META\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x00,KEY_MOD_LMETA,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;

        case am_state_t::COMMAND_L_S:
            PRINT("COMMAND_L_S: %c\n",m_am_str[m_am_pos]);
            switch (m_am_str[m_am_pos])
            {
                case 'H':
                    m_am_state = COMMAND_L_SH;
                    break;
                case 'T':
                    m_am_state = COMMAND_L_ST;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_L_SH:
            PRINT("COMMAND_L_SH\n");
            switch (m_am_str[m_am_pos])
            {
                case 'I':
                    m_am_state = COMMAND_L_SHI;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_L_SHI:
            PRINT("COMMAND_L_SHI\n");
            switch (m_am_str[m_am_pos])
            {
                case 'F':
                    m_am_state = COMMAND_L_SHIF;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_L_SHIF:
            PRINT("COMMAND_L_SHIF\n");
            switch (m_am_str[m_am_pos])
            {
                case 'T':
                    m_am_state = COMMAND_L_SHIFT;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_L_SHIFT:
            PRINT("COMMAND_L_SHIFT\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x00,KEY_MOD_LSHIFT,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        case am_state_t::COMMAND_L_ST:
            PRINT("COMMAND_L_ST\n");
            switch (m_am_str[m_am_pos])
            {
                case 'R':
                    m_am_state = COMMAND_L_STR;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_STR:
            PRINT("COMMAND_L_STR\n");
            switch (m_am_str[m_am_pos])
            {
                case 'G':
                    m_am_state = COMMAND_L_STRG;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_L_STRG:
            PRINT("COMMAND_L_STRG\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x00,KEY_MOD_LCTRL,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;

        // Command NULL
        case am_state_t::COMMAND_N:
            PRINT("COMMAND_N\n");
            switch (m_am_str[m_am_pos])
            {
                case 'U':
                    m_am_state = COMMAND_NU;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_NU:
            PRINT("COMMAND_NU\n");
            switch (m_am_str[m_am_pos])
            {
                case 'L':
                    m_am_state = COMMAND_NUL;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_NUL:
            PRINT("COMMAND_NUL\n");
            switch (m_am_str[m_am_pos])
            {
                case 'L':
                    m_am_state = COMMAND_NULL;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_NULL:
            PRINT("COMMAND_NULL\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_null_node(RID_KEYBOARD);  // use Keyboard rid by default
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        
        // COMMAND OE
        case am_state_t::COMMAND_O:
            PRINT("COMMAND_O\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_OE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_OE:
            PRINT("COMMAND_OE\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x33,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;

        // COMMAND R_ALT/R_KEY/R_META/R_SHIFT
        case am_state_t::COMMAND_R:
            PRINT("COMMAND_R\n");
            switch (m_am_str[m_am_pos])
            {
                case '_':
                    m_am_state = COMMAND_R_;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_:
            PRINT("COMMAND_R_\n");
            switch (m_am_str[m_am_pos])
            {
                case 'A':
                    m_am_state = COMMAND_R_A;
                    break;
                case 'K':
                    m_am_state = COMMAND_R_K;
                    break;
                case 'M':
                    m_am_state = COMMAND_R_M;
                    break;
                case 'S':
                    m_am_state = COMMAND_R_S;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_A:
            PRINT("COMMAND_R_A\n");
            switch (m_am_str[m_am_pos])
            {
                case 'L':
                    m_am_state = COMMAND_R_AL;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_AL:
            PRINT("COMMAND_R_AL\n");
            switch (m_am_str[m_am_pos])
            {
                case 'T':
                    m_am_state = COMMAND_R_ALT;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_ALT:
            PRINT("COMMAND_R_ALT\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x00,KEY_MOD_RALT,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_K:
            PRINT("COMMAND_R_K\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_R_KE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_KE:
            PRINT("COMMAND_R_KE\n");
            switch (m_am_str[m_am_pos])
            {
                case 'Y':
                    m_am_state = COMMAND_R_KEY;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_KEY:
            PRINT("COMMAND_R_KEY\n");
            switch (m_am_str[m_am_pos])
            {
                case '{':
                    m_num_1 = 0;
                    m_am_state = COMMAND_L_KEY_BR_OPEN;
                    break;
                default:
                    bool success = append_keycode(HID_KEY_ARROW_RIGHT,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_KEY_BR_OPEN:
            PRINT("COMMAND_R_KEY_BR_OPEN\n");
            if(m_am_str[m_am_pos] >= '0' && m_am_str[m_am_pos] <= '9')
            {
                m_num_1 *= 10;
                m_num_1 += m_am_str[m_am_pos] - '0';
            }
            else
            {
                m_am_state = COMMAND_R_KEY_BR_CLOSE;
            }
            break;
        case am_state_t::COMMAND_R_KEY_BR_CLOSE:
            PRINT("COMMAND_R_KEY_BR_CLOSE\n");
            for(int i = 0; i < m_num_1; i++)
            {
                bool success = append_keycode(HID_KEY_ARROW_RIGHT,0,RID_KEYBOARD);
                m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
            }
            next_char = false;
            break;

        case am_state_t::COMMAND_R_M:
            PRINT("COMMAND_R_M\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_R_ME;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_ME:
            PRINT("COMMAND_R_ME\n");
            switch (m_am_str[m_am_pos])
            {
                case 'T':
                    m_am_state = COMMAND_R_MET;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_MET:
            PRINT("COMMAND_R_MET\n");
            switch (m_am_str[m_am_pos])
            {
                case 'A':
                    m_am_state = COMMAND_R_META;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_META:
            PRINT("COMMAND_R_META\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x00,KEY_MOD_RMETA,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;

        case am_state_t::COMMAND_R_S:
            PRINT("COMMAND_R_S\n");
            switch (m_am_str[m_am_pos])
            {
                case 'H':
                    m_am_state = COMMAND_R_SH;
                    break;
                case 'T':
                    m_am_state = COMMAND_R_ST;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_R_SH:
            PRINT("COMMAND_R_SH\n");
            switch (m_am_str[m_am_pos])
            {
                case 'I':
                    m_am_state = COMMAND_R_SHI;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_R_SHI:
            PRINT("COMMAND_R_SHI\n");
            switch (m_am_str[m_am_pos])
            {
                case 'F':
                    m_am_state = COMMAND_R_SHIF;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_R_SHIF:
            PRINT("COMMAND_R_SHIF\n");
            switch (m_am_str[m_am_pos])
            {
                case 'T':
                    m_am_state = COMMAND_R_SHIFT;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;

        case am_state_t::COMMAND_R_SHIFT:
            PRINT("COMMAND_R_SHIFT\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x00,KEY_MOD_RSHIFT,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;

        case am_state_t::COMMAND_R_ST:
            PRINT("COMMAND_R_ST\n");
            switch (m_am_str[m_am_pos])
            {
                case 'R':
                    m_am_state = COMMAND_R_STR;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_STR:
            PRINT("COMMAND_R_STR\n");
            switch (m_am_str[m_am_pos])
            {
                case 'G':
                    m_am_state = COMMAND_R_STRG;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_R_STRG:
            PRINT("COMMAND_R_STRG\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x00,KEY_MOD_RCTRL,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;

        // COMMAND STRG/SZ
        case am_state_t::COMMAND_S:
            PRINT("COMMAND_S\n");
            switch (m_am_str[m_am_pos])
            {
                case 'T':
                    m_am_state = COMMAND_ST;
                    break;
                case 'Z':
                    m_am_state = COMMAND_SZ;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_ST:
            PRINT("COMMAND_ST\n");
            switch (m_am_str[m_am_pos])
            {
                case 'R':
                    m_am_state = COMMAND_STR;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_STR:
            PRINT("COMMAND_STR\n");
            switch (m_am_str[m_am_pos])
            {
                case 'G':
                    m_am_state = COMMAND_STRG;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_STRG:
            PRINT("COMMAND_STRG\n");
            switch (m_am_str[m_am_pos])
            {
                case '{':
                    m_am_state = COMMAND_STRG_BR_OPEN;
                    break;
                default:
                    bool success = append_keycode('\0',KEY_MOD_LCTRL,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        case am_state_t::COMMAND_STRG_BR_OPEN:
            PRINT("COMMAND_STRG_BR_OPEN\n");
            switch (m_am_str[m_am_pos])
            {
                case '}':
                    m_am_state = am_state_t::IDLE;
                    break;
                default:
                    bool success = append_keycode(ascii_to_keycode(m_am_str[m_am_pos]),KEY_MOD_LCTRL,RID_KEYBOARD);
                    m_am_state = success ? m_am_state : am_state_t::ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_SZ:
            PRINT("COMMAND_SZ\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x2D,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;

		// COMMAND TAB
        case am_state_t::COMMAND_T:
			PRINT("COMMAND_T\n");
			switch (m_am_str[m_am_pos])
			{
				case 'A':
					m_am_state = COMMAND_TA;
					break;
				default:
					m_am_state = ERROR;
					break;
			}
			break;
		case am_state_t::COMMAND_TA:
			PRINT("COMMAND_TA\n");
			switch (m_am_str[m_am_pos])
			{
				case 'B':
					m_am_state = COMMAND_TAB;
					break;
				default:
					m_am_state = ERROR;
					break;
			}
			break;
		case am_state_t::COMMAND_TAB:
			PRINT("COMMAND_TAB\n");
			switch (m_am_str[m_am_pos])
			{
				default:
					bool success = append_keycode(0x2B,0,RID_KEYBOARD);
					m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
					next_char = false;
					break;
			}
			break;

        // COMMAND UP_KEY/UE
        case am_state_t::COMMAND_U:
            PRINT("COMMAND_U\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_UE;
                    break;
                case 'P':
                    m_am_state = COMMAND_UP;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_UE:
            PRINT("COMMAND_UE\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode(0x2F,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        case am_state_t::COMMAND_UP:
            PRINT("COMMAND_UP\n");
            switch (m_am_str[m_am_pos])
            {
                case '_':
                    m_am_state = COMMAND_UP_;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_UP_:
            PRINT("COMMAND_UP_\n");
            switch (m_am_str[m_am_pos])
            {
                case 'K':
                    m_am_state = COMMAND_UP_K;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_UP_K:
            PRINT("COMMAND_UP_K\n");
            switch (m_am_str[m_am_pos])
            {
                case 'E':
                    m_am_state = COMMAND_UP_KE;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_UP_KE:
            PRINT("COMMAND_UP_KE\n");
            switch (m_am_str[m_am_pos])
            {
                case 'Y':
                    m_am_state = COMMAND_UP_KEY;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_UP_KEY:
            PRINT("COMMAND_UP_KEY\n");
            switch (m_am_str[m_am_pos])
            {
                case '{':
                    m_num_1 = 0;
                    m_am_state = COMMAND_UP_KEY_BR_OPEN;
                    break;
                default:
                    bool success = append_keycode(HID_KEY_ARROW_UP,0,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        case am_state_t::COMMAND_UP_KEY_BR_OPEN:
            PRINT("COMMAND_UP_KEY_BR_OPEN\n");
            if(m_am_str[m_am_pos] >= '0' && m_am_str[m_am_pos] <= '9')
            {
                m_num_1 *= 10;
                m_num_1 += m_am_str[m_am_pos] - '0';
            }
            else
            {
                m_am_state = COMMAND_UP_KEY_BR_CLOSE;
            }
            break;
        case am_state_t::COMMAND_UP_KEY_BR_CLOSE:
            PRINT("COMMAND_UP_KEY_BR_CLOSE\n");
            for(int i = 0; i < m_num_1; i++)
            {
                bool success = append_keycode(HID_KEY_ARROW_UP,0,RID_KEYBOARD);
                m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
            }
            next_char = false;
            break;

        default:
            m_am_state = am_state_t::ERROR;
            break;
        }
        if(next_char)
            am_next_char();
    }



    return false;
}

bool animacro_parser::am_color_parse(const char* c_str, color_t* color)
{
    PRINT("color_parse: %s\n", c_str);
    if(c_str == nullptr)
        return false;
    if(c_str[0] == '\0')  // If empty String is passed...Counts more as DONE than ERROR...
        return true;
    if(color == nullptr)
        return false;


    if(strcmp(c_str, "blue") == 0 || strcmp(c_str, "BLUE") == 0 || strcmp(c_str, "Blue") == 0)
    {
        *color = {.r=0,.g=0,.b=255,.a=0};
        return true;
    }

    if(strcmp(c_str, "cyan") == 0 || strcmp(c_str, "CYAN") == 0 || strcmp(c_str, "Cyan") == 0)
    {
        *color = {.r=0,.g=255,.b=255,.a=0};
        return true;
    }

    if(strcmp(c_str, "green") == 0 || strcmp(c_str, "GREEN") == 0 || strcmp(c_str, "Green") == 0)
    {
        *color = {.r=0,.g=255,.b=0,.a=0};
        return true;
    }

    if(strcmp(c_str, "magenta") == 0 || strcmp(c_str, "MAGENTA") == 0 || strcmp(c_str, "Magenta") == 0)
    {
        *color = {.r=255,.g=0,.b=255,.a=0};
        return true;
    }

    if(strcmp(c_str, "orange") == 0 || strcmp(c_str, "ORANGE") == 0 || strcmp(c_str, "Orange") == 0)
    {
        *color = {.r=255,.g=165,.b=0,.a=0};
        return true;
    }

    if(strcmp(c_str, "purple") == 0 || strcmp(c_str, "PURPLE") == 0 || strcmp(c_str, "Purple") == 0)
    {
        *color = {.r=255,.g=0,.b=255,.a=0};
        return true;
    }

    if(strcmp(c_str, "red") == 0 || strcmp(c_str, "RED") == 0 || strcmp(c_str, "Red") == 0)
    {
        *color = {.r=255,.g=0,.b=0,.a=0};
        return true;
    }
    if(strcmp(c_str, "white") == 0 || strcmp(c_str, "WHITE") == 0 || strcmp(c_str, "White") == 0)
    {
        *color = {.r=255,.g=255,.b=255,.a=0};
        return true;
    }

    if(strcmp(c_str, "yellow") == 0 || strcmp(c_str, "YELLOW") == 0 || strcmp(c_str, "Yellow") == 0)
    {
        *color = {.r=255,.g=255,.b=0,.a=0};
        return true;
    }

    return false;
}

bool animacro_parser::am_layer_color_effect_parse(const char* c_str, layer_color_effect_t* effect)
{
    PRINT("layer_color_effect_parse: %s\n", c_str);
    if(c_str[0] == '\0')  // If empty String is passed...Counts more as DONE than ERROR...
        return true;
    if(effect == nullptr)
        return false;

    if(strcmp(c_str, "none") == 0 || strcmp(c_str, "NONE") == 0 || strcmp(c_str, "None") == 0)
    {
        *effect = layer_color_effect_t::NONE;
        return true;
    }

    if(strcmp(c_str, "rainbow") == 0 || strcmp(c_str, "RAINBOW") == 0 || strcmp(c_str, "Rainbow") == 0)
    {
        *effect = layer_color_effect_t::RAINBOW;
        return true;
    }

    if(strcmp(c_str, "const") == 0 || strcmp(c_str, "CONST") == 0 || strcmp(c_str, "Const") == 0)
    {
        *effect = layer_color_effect_t::CONST_COLOR;
        return true;
    }

    return false;
}

bool animacro_parser::am_display_color_effect_parse(const char* c_str, display_color_effect_t* effect)
{
    PRINT("display_color_effect_parse: %s\n", c_str);
    if(c_str[0] == '\0')  // If empty String is passed...Counts more as DONE than ERROR...
        return true;
    if(effect == nullptr)
        return false;

    if(strcmp(c_str, "none") == 0 || strcmp(c_str, "NONE") == 0 || strcmp(c_str, "None") == 0)
    {
        *effect = display_color_effect_t::NONE;
        return true;
    }

    if(strcmp(c_str, "rainbow") == 0 || strcmp(c_str, "RAINBOW") == 0 || strcmp(c_str, "Rainbow") == 0)
    {
        *effect = display_color_effect_t::RAINBOW;
        return true;
    }

    if(strcmp(c_str, "const") == 0 || strcmp(c_str, "CONST") == 0 || strcmp(c_str, "Const") == 0)
    {
        *effect = display_color_effect_t::CONST_COLOR;
        return true;
    }

    return false;
}

bool animacro_parser::am_display_render_mode_parse(const char* c_str, led_wall_render_mode* mode)
{
    PRINT("display_mode_parse: %s\n", c_str);
    if(c_str[0] == '\0')  // If empty String is passed...Counts more as DONE than ERROR...
        return true;
    if(mode == nullptr)
        return false;

    if(strcmp(c_str, "text") == 0 || strcmp(c_str, "TEXT") == 0 || strcmp(c_str, "Text") == 0)
    {
        *mode = led_wall_render_mode::TEXT;
        return true;
    }

    if(strcmp(c_str, "gif") == 0 || strcmp(c_str, "GIF") == 0 || strcmp(c_str, "Gif") == 0)
    {
        *mode = led_wall_render_mode::GIF;
        return true;
    }

    return false;
}

bool animacro_parser::am_key_color_effect_parse(const char* c_str, key_color_effect_t* effect)
{
    PRINT("key_color_effect_parse: %s\n", c_str);
    if(c_str[0] == '\0')  // If empty String is passed...Counts more as DONE than ERROR...
        return true;
    if(effect == nullptr)
        return false;

    if(strcmp(c_str, "none") == 0 || strcmp(c_str, "NONE") == 0 || strcmp(c_str, "None") == 0)
    {
        *effect = key_color_effect_t::NONE;
        return true;
    }

    if(strcmp(c_str, "pressed") == 0 || strcmp(c_str, "PRESSED") == 0 || strcmp(c_str, "Pressed") == 0)
    {
        *effect = key_color_effect_t::PRESSED;
        return true;
    }

    if(strcmp(c_str, "released") == 0 || strcmp(c_str, "RELEASED") == 0 || strcmp(c_str, "Released") == 0)
    {
        *effect = key_color_effect_t::RELEASED;
        return true;
    }

    if(strcmp(c_str, "pressed&released") == 0 || strcmp(c_str, "PRESSED&RELEASED") == 0 || strcmp(c_str, "Pressed&Released") == 0)
    {
        *effect = key_color_effect_t::PRESSED_AND_RELEASED;
        return true;
    }

    if(strcmp(c_str, "const") == 0 || strcmp(c_str, "CONST") == 0 || strcmp(c_str, "Const") == 0)
    {
        *effect = key_color_effect_t::CONST_COLOR;
        return true;
    }

    return false;
}


void animacro_parser::am_next_char()
{
    if(m_am_str[m_am_pos] == '\0')
    {
        m_am_state = m_am_state == am_state_t::IDLE ? am_state_t::DONE : am_state_t::ERROR;
        return;
    }

    m_am_pos++;
}


bool animacro_parser::append_keycode(char c)
{
    return append_keycode(ascii_to_keycode(c), ascii_to_mod(c), RID_KEYBOARD);
}

bool animacro_parser::append_keycode(uint8_t keycode, uint8_t mod, uint8_t r_id)
{
    keycode_node * node = get_leaf(m_am_key->get_keycode_root());
    if(node == nullptr)
        return false;

    // Check for first/malformed node
    if(node->r_id == (uint8_t)-1)
    {
        PRINT("node->r_id: %d\n",node->r_id);
        node->r_id = r_id;
    }
    
    bool keycode_exists = false;
    for(int i = 0; i < 6; i++)
    {
        if(keycode == '\0') // Edgecase for no Keycode provided
        {
            keycode_exists = false;
            break;
        }
        if(keycode == node->codes[i])
        {
            keycode_exists = true;
            break;
        }
    }

    // Case 1: Keycode to be added already exists in current node.
    //     Do: Append empty node to release all keys and then start new node
    if(keycode_exists)
    {
        PRINT("\tCASE 1\n");
        node = push_back_node(node);    // 1. Empty node
        if(node == nullptr)
            return false;
        node->r_id = r_id;

        node = push_back_node(node);    // 2. New node
        if(node == nullptr)
            return false;
        node->r_id = r_id;
        node->codes[0] = keycode;
        node->mod = mod;
    }

    // Case 2.1: RID differs from the one set in node.
    //     Do: Start new node
    else if(node->r_id != r_id)
    {
        PRINT("\tCASE 2.1\n");
        uint8_t r_id_old = node->r_id;
        node = push_back_node(node);    // 1. Empty node with old RID
        if(node == nullptr)
            return false;
        node->r_id = r_id_old;

        node = push_back_node(node);    // 2. New node
        if(node == nullptr)
            return false;
        node->r_id = r_id;
        node->codes[0] = keycode;
        node->mod = mod;
    }

    // Case 2.2: Modifier differs from the one set in node and keycode buffer is not empty.
    //     Do: Start new node
    else if(node->mod != mod && node->codes[0] != 0)
    {
        PRINT("\tCASE 2.2\n");
        node = push_back_node(node);    // 1. Empty node (is mostly overkill, but if same keycode is repeated in next packet (just with different modifier), the key is never released and not sent! e.g.: 'Rr' would only send 'R' and NOT the second 'r')
        if(node == nullptr)
            return false;
        node->r_id = r_id;

        node = push_back_node(node);    // 2. New node
        if(node == nullptr)
            return false;
        node->r_id = r_id;
        node->codes[0] = keycode;
        node->mod = mod;
    }

    //Case 3: Codes still has space and other cases did not trigger.
    //    Do: Append to codes
    else if(node->codes[5] == 0)
    {
        PRINT("\tCASE 3\n");
        for(int i = 0; i < 6; i++)
        {
            if(node->codes[i] == 0)
            {
                node->codes[i] = keycode;
                break;
            }
        }
        node->mod = mod;
        node->r_id = r_id;
    }

    // Case 4: Codes full
    //     Do: Start new node
    else
    {
        PRINT("\tCASE 4\n");
        node = push_back_node(node);    // 1. Empty node
        if(node == nullptr)
            return false;
        node->r_id = r_id;

        node = push_back_node(node);    // 2. New node
        if(node == nullptr)
            return false;
        node->r_id = r_id;
        node->codes[0] = keycode;
        node->mod = mod;
    }

    if(node->r_id == (uint8_t)-1)
        node->r_id = r_id;

    return true;
}

bool animacro_parser::append_null_node(uint8_t r_id)
{
    keycode_node * node = get_leaf(m_am_key->get_keycode_root());
    if(node == nullptr)
        return false;

    // Need to append 2 Nodes, as the last empty node will be filled by the parser.
    // Only use this function if further parsing data.
    // I mean, it wont hurt anything to add redundant NULL nodes, but its kinda useless...

    node = push_back_node(node);    // 1. Empty Node
    if(node == nullptr)
        return false;
    node->r_id = r_id;

    node = push_back_node(node);    // 2. New Node
    if(node == nullptr)
        return false;
    node->r_id = r_id;

    return true;
}

bool animacro_parser::append_delay_node(uint32_t delay_us)
{
    keycode_node * node = get_leaf(m_am_key->get_keycode_root());
    if(node == nullptr)
        return false;

    uint8_t r_id_old = node->r_id;
    node = push_back_node(node);    // 1. Empty Node (assume previous node was not a delay node and clear other r_id keycodes)
    if(node == nullptr)
        return false;
    node->r_id = r_id_old;

    node = push_back_node(node);    // 2. Delay node
    if(node == nullptr)
        return false;
    node->r_id = RID_DELAY;
    node->codes[0] = (delay_us & 0xFF000000)>>24;
    node->codes[1] = (delay_us & 0x00FF0000)>>16;
    node->codes[2] = (delay_us & 0x0000FF00)>>8;
    node->codes[3] = (delay_us & 0x000000FF);
    PRINT("codes[0]: %d\n", node->codes[0]);
    PRINT("codes[1]: %d\n", node->codes[1]);
    PRINT("codes[2]: %d\n", node->codes[2]);
    PRINT("codes[3]: %d\n", node->codes[3]);

    return true;
}
