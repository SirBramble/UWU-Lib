#include "animacro.h"
#include "color.h"
#include "memory.h"
#include "report_id.h"
#include <cstring>

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
    printf("data_parse: %s\n", c_str);
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
            printf("IDLE: %c\n", m_am_str[m_am_pos]);
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
            switch (m_am_str[m_am_pos])
            {
                case 'S':
                    m_am_state = COMMAND_S;
                    break;
                case 'N':
                    m_am_state = COMMAND_N;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::DONE:
            printf("DONE\n");
            return true;
            break;
        case am_state_t::ERROR:
            printf("ERROR\n");
            return false;
            break;
        // *************** COMMAND PARSE *************** //
        case am_state_t::COMMAND_N:
            printf("COMMAND_N\n");
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
            printf("COMMAND_NU\n");
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
            printf("COMMAND_NUL\n");
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
            printf("COMMAND_NULL\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_null_node(RID_KEYBOARD);  // use Keyboard rid by default
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        case am_state_t::COMMAND_S:
            printf("COMMAND_S\n");
            switch (m_am_str[m_am_pos])
            {
                case 'T':
                    m_am_state = COMMAND_ST;
                    break;
                default:
                    m_am_state = ERROR;
                    break;
            }
            break;
        case am_state_t::COMMAND_ST:
            printf("COMMAND_ST\n");
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
            printf("COMMAND_STR\n");
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
            printf("COMMAND_STRG\n");
            switch (m_am_str[m_am_pos])
            {
                default:
                    bool success = append_keycode('\0',KEY_MOD_LCTRL,RID_KEYBOARD);
                    m_am_state = success ? am_state_t::IDLE : am_state_t::ERROR;
                    next_char = false;
                    break;
            }
            break;
        default:
            break;
        }
        if(next_char)
            am_next_char();
    }



    return false;
}

bool animacro_parser::am_color_parse(const char* c_str, color_t* color)
{
    printf("color_parse: %s\n", c_str);
    if(c_str == nullptr)
        return false;
    if(c_str[0] == '\0')  // If empty String is passed...Counts more as DONE than ERROR...
        return true;
    if(color == nullptr)
        return false;


    if(strcmp(c_str, "blue") == 0 || strcmp(c_str, "BLUE") == 0 || strcmp(c_str, "Blue") == 0)
    {
        *color = {0,0,255,0};
        return true;
    }

    if(strcmp(c_str, "cyan") == 0 || strcmp(c_str, "CYAN") == 0 || strcmp(c_str, "Cyan") == 0)
    {
        *color = {0,255,255,0};
        return true;
    }

    if(strcmp(c_str, "green") == 0 || strcmp(c_str, "GREEN") == 0 || strcmp(c_str, "Green") == 0)
    {
        *color = {0,255,0,0};
        return true;
    }

    if(strcmp(c_str, "magenta") == 0 || strcmp(c_str, "MAGENTA") == 0 || strcmp(c_str, "Magenta") == 0)
    {
        *color = {255,0,255,0};
        return true;
    }

    if(strcmp(c_str, "orange") == 0 || strcmp(c_str, "ORANGE") == 0 || strcmp(c_str, "Orange") == 0)
    {
        *color = {255,165,0,0};
        return true;
    }

    if(strcmp(c_str, "purple") == 0 || strcmp(c_str, "PURPLE") == 0 || strcmp(c_str, "Purple") == 0)
    {
        *color = {255,0,255,0};
        return true;
    }

    if(strcmp(c_str, "red") == 0 || strcmp(c_str, "RED") == 0 || strcmp(c_str, "Red") == 0)
    {
        *color = {255,0,0,0};
        return true;
    }
    if(strcmp(c_str, "white") == 0 || strcmp(c_str, "WHITE") == 0 || strcmp(c_str, "White") == 0)
    {
        *color = {255,255,255,0};
        return true;
    }

    if(strcmp(c_str, "yellow") == 0 || strcmp(c_str, "YELLOW") == 0 || strcmp(c_str, "Yellow") == 0)
    {
        *color = {255,255,0,0};
        return true;
    }



    return false;
}

bool animacro_parser::am_layer_color_effect_parse(const char* c_str, layer_color_effect_t* effect)
{
    printf("layer_color_effect_parse: %s\n", c_str);
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

bool animacro_parser::am_key_color_effect_parse(const char* c_str, key_color_effect_t* effect)
{
    printf("key_color_effect_parse: %s\n", c_str);
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
        printf("\tCASE 1\n");
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

    // Case 2: Modifier differs from the one set in node and keycode buffer is not empty.
    //     Do: Start new node
    else if(node->mod != mod && node->codes[0] != 0)
    {
        printf("\tCASE 2\n");
        node = push_back_node(node);    // 1. New node
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
        printf("\tCASE 3\n");
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
        printf("\tCASE 4\n");
        node = push_back_node(node);    // 1. New node
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

    // Need to append 2 Nodes, as the last empty node will be filled by the paser.
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
