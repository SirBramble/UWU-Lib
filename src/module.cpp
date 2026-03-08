#include "module.h"
#include "color.h"
#include "layer.h"
#include "lwjson.h"

#if IS_MCU_VERSION == 0
    #include <cstdlib>
    #include <stdio.h>
    #define PRINT(...) printf(__VA_ARGS__)
#else
    #include <Arduino.h>
    #define PRINT(...) Serial.printf(__VA_ARGS__)
#endif

using namespace uwu;

const int c_stack_pos_name        = 1;
const int c_stack_pos_layer_level = 4;
const int c_stack_pos_key_arg     = 7;
const int c_stack_pos_rgb         = 9;



#if IS_MCU_VERSION != 0

void _module::update_keymap_from_file(FatVolume &volume)
{
    PRINT("this: %p", (void*)this);

    File32 file = volume.open(CONFIG_FILE_PATH, O_RDONLY);

    if (!file) {
        Serial.println("Open file failed");
        return;
    }

    Serial.printf("Reading %s", CONFIG_FILE_PATH);

    lwjsonr_t res;
    lwjson_stream_init(&m_stream_parser, &_module::update_keymap_json_callback);
    lwjson_stream_set_user_data(&m_stream_parser, this);    // link _module object to stream parser struct

    while (file.available()) {
        char c = file.read();
        res = lwjson_stream_parse(&m_stream_parser, c);
        if (res == lwjsonSTREAMINPROG) {
        } else if (res == lwjsonSTREAMWAITFIRSTCHAR) {
            PRINT("Waiting first character\r\n");
        } else if (res == lwjsonSTREAMDONE) {
            PRINT("Done\r\n");
        } else {
            PRINT("JSON Error\r\n");
            break;
        }
    }

    PRINT("Parsing completed\r\n");

    file.close();
}

#endif


void _module::update_keymap(const char* json_str)
{
    PRINT("this: %p", (void*)this);

    lwjsonr_t res;
    lwjson_stream_init(&m_stream_parser, &_module::update_keymap_json_callback);
    lwjson_stream_set_user_data(&m_stream_parser, this);    // link _module object to stream parser struct

    for (const char* c = json_str; *c != '\0'; ++c) {
        res = lwjson_stream_parse(&m_stream_parser, *c);
        if (res == lwjsonSTREAMINPROG) {
        } else if (res == lwjsonSTREAMWAITFIRSTCHAR) {
            PRINT("Waiting first character\r\n");
        } else if (res == lwjsonSTREAMDONE) {
            PRINT("Done\r\n");
        } else {
            PRINT("JSON Error\r\n");
            break;
        }
    }
    PRINT("Parsing completed\r\n");
}


void _module::update_keymap_json_callback(lwjson_stream_parser_t* jsp, lwjson_stream_type_t type)
{
    // This is some fun casting BS to get the module mapped to the passed 'jsp', as the callback function has to be static.
    // It could just be defined in the module.cpp but is mapped to the '_module' class for convenience.
    auto* self = static_cast<_module*>(lwjson_stream_get_user_data(jsp));
    self->update_keymap_handle_json_callback(jsp,type);
    (void)type;
}

void _module::update_keymap_handle_json_callback(lwjson_stream_parser_t* jsp, lwjson_stream_type_t type)
{
    // if(type == LWJSON_STREAM_TYPE_OBJECT_END || type == LWJSON_STREAM_TYPE_ARRAY_END)
    //     return; // No need to parse end tokens

    PRINT("t: %d\t",type);
    PRINT("stack_pos: %zu\t",jsp->stack_pos);
    for(int i = 0; i < 16; i++)
    {
        if (jsp->stack[i].type == LWJSON_STREAM_TYPE_ARRAY || jsp->stack[i].type == LWJSON_STREAM_TYPE_ARRAY_END)
            PRINT("s%d:%d\t",i,jsp->stack[i].meta.index);
        else if (jsp->stack[i].type == LWJSON_STREAM_TYPE_NONE)
            PRINT("s%d:N/U\t",i);
        else
            PRINT("s%d:%s\t",i,jsp->stack[i].meta.name);
    }
    PRINT("buf: %s\n",jsp->data.str.buff);


    /* Check for module match ******* if stack_pos more than or equal 2, can index safely */
    if (!(jsp->stack_pos >=2 && lwjson_stack_seq_2(jsp, 0, OBJECT, KEY) && strcmp(jsp->stack[c_stack_pos_name].meta.name, m_name) == 0))
    {
        return;
    }
    // printf("Got key '%s' with value '%s'\r\n", jsp->stack[jsp->stack_pos-1].meta.name, jsp->data.str.buff);


    /******************* Layer Stuff *******************/
    /* Check for Layer number */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_NUMBER && strcmp(jsp->stack[jsp->stack_pos-1].meta.name, "layer") == 0)
    {
        m_parser_layer = atoi(jsp->data.str.buff);
        // printf("layer: %d\n", m_parser_layer);
        return;
    }

    /* Check for layer color (RGB Object) */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_NUMBER && strcmp(jsp->stack[c_stack_pos_layer_level].meta.name, "color") == 0)
    {
        PRINT("layer rgb [%d]: %s\n", jsp->stack[jsp->stack_pos-1].meta.index, jsp->data.str.buff);

        if(m_parser_layer >= MAX_NUM_LAYERS)
            return;

        _layer* l = get_layer(m_parser_layer);

        if(jsp->stack[jsp->stack_pos-1].meta.index == 0)
            l->set_color_r(atoi(jsp->data.str.buff));
        else if(jsp->stack[jsp->stack_pos-1].meta.index == 1)
            l->set_color_g(atoi(jsp->data.str.buff));
        else if(jsp->stack[jsp->stack_pos-1].meta.index == 2)
            l->set_color_b(atoi(jsp->data.str.buff));

        return;
    }

    /* Check for layer color effect (String) */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_STRING && strcmp(jsp->stack[c_stack_pos_layer_level].meta.name, "color_effect") == 0)
    {
        PRINT("layer effect: %s\n", jsp->data.str.buff);

        if(m_parser_layer >= MAX_NUM_LAYERS)
            return;

        _layer* l = get_layer(m_parser_layer);

        layer_color_effect_t e;

        am_layer_color_effect_parse(jsp->data.str.buff, &e);

        l->set_effect(e);

        return;
    }

    /* Check for layer color effect speed (Int) */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_NUMBER && strcmp(jsp->stack[c_stack_pos_layer_level].meta.name, "color_effect_speed") == 0)
    {
        PRINT("layer rgb effect speed: %d\n", atoi(jsp->data.str.buff));

        if(m_parser_layer >= MAX_NUM_LAYERS)
            return;

        _layer* l = get_layer(m_parser_layer);

        l->set_effect_speed(atoi(jsp->data.str.buff));

        return;
    }


    /******************* Key Stuff *******************/
    /* Check for key number */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_NUMBER && strcmp(jsp->stack[c_stack_pos_layer_level].meta.name, "keys") == 0 && strcmp(jsp->stack[jsp->stack_pos-1].meta.name, "index") == 0)
    {
        m_parser_key = atoi(jsp->data.str.buff);
        // printf("key: %d\n", m_parser_key);
        return;
    }

    /* Check for key data */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_STRING && strcmp(jsp->stack[c_stack_pos_layer_level].meta.name, "keys") == 0 && strcmp(jsp->stack[jsp->stack_pos-1].meta.name, "data") == 0)
    {
        key* k = get_key(m_parser_key, m_parser_layer);

        if(k == nullptr)
            return;

        // If compiled for PC save a copy of the data in the datastructure for later reconstruction
        #if (IS_MCU_VERSION == 0)
        k->set_data(jsp->data.str.buff);
        #endif

        PRINT("data: %s\n", jsp->data.str.buff);

        bool success = am_data_parse(jsp->data.str.buff, k);
        k->set_parse_error(!success);

        return;
    }

    /* Check for key color_effect (String) */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_STRING && strcmp(jsp->stack[c_stack_pos_layer_level].meta.name, "keys") == 0 && strcmp(jsp->stack[jsp->stack_pos-1].meta.name, "color_effect") == 0)
    {
        key* k = get_key(m_parser_key, m_parser_layer);
        if(k == nullptr)
            return;

        PRINT("color_effect: %s\n", jsp->data.str.buff);

        key_color_effect_t color_effect;
        bool success = am_key_color_effect_parse(jsp->data.str.buff, &color_effect);
        k->set_color_effect(color_effect);
        k->set_parse_error(!success);

        return;
    }

    /* Check for key color (String) */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_STRING && strcmp(jsp->stack[c_stack_pos_layer_level].meta.name, "keys") == 0 && strcmp(jsp->stack[jsp->stack_pos-1].meta.name, "color") == 0)
    {
        key* k = get_key(m_parser_key, m_parser_layer);
        if(k == nullptr)
            return;

        PRINT("color: %s\n", jsp->data.str.buff);

        color_t color;
        bool success = am_color_parse(jsp->data.str.buff, &color);
        k->set_color(color);
        k->set_parse_error(!success);

        return;
    }


    /* Check for key color (RGB Object) */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_NUMBER && strcmp(jsp->stack[c_stack_pos_key_arg].meta.name, "color") == 0)
    {
        PRINT("rgb [%d]: %s\n", jsp->stack[jsp->stack_pos-1].meta.index, jsp->data.str.buff);

        key* k = get_key(m_parser_key, m_parser_layer);
        if(k == nullptr)
            return;

        if(jsp->stack[jsp->stack_pos-1].meta.index == 0)
            k->set_color_r(atoi(jsp->data.str.buff));
        else if(jsp->stack[jsp->stack_pos-1].meta.index == 1)
            k->set_color_g(atoi(jsp->data.str.buff));
        else if(jsp->stack[jsp->stack_pos-1].meta.index == 2)
            k->set_color_b(atoi(jsp->data.str.buff));

        return;
    }

    /* Check for key color_pressed (String) */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_STRING && strcmp(jsp->stack[c_stack_pos_layer_level].meta.name, "keys") == 0 && strcmp(jsp->stack[jsp->stack_pos-1].meta.name, "color_pressed") == 0)
    {

        key* k = get_key(m_parser_key, m_parser_layer);
        if(k == nullptr)
            return;

        PRINT("color: %s\n", jsp->data.str.buff);

        color_t color;
        bool success = am_color_parse(jsp->data.str.buff, &color);
        k->set_color_pressed(color);
        k->set_parse_error(!success);

        return;
    }


    /* Check for key color_pressed (RGB Object) */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_NUMBER && strcmp(jsp->stack[c_stack_pos_key_arg].meta.name, "color_pressed") == 0)
    {
        PRINT("rgb [%d]: %s\n", jsp->stack[jsp->stack_pos-1].meta.index, jsp->data.str.buff);

        key* k = get_key(m_parser_key, m_parser_layer);
        if(k == nullptr)
            return;

        if(jsp->stack[jsp->stack_pos-1].meta.index == 0)
            k->set_color_pressed_r(atoi(jsp->data.str.buff));
        else if(jsp->stack[jsp->stack_pos-1].meta.index == 1)
            k->set_color_pressed_g(atoi(jsp->data.str.buff));
        else if(jsp->stack[jsp->stack_pos-1].meta.index == 2)
            k->set_color_pressed_b(atoi(jsp->data.str.buff));

        return;
    }

    /* Check for key color_released (String) */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_STRING && strcmp(jsp->stack[c_stack_pos_layer_level].meta.name, "keys") == 0 && strcmp(jsp->stack[jsp->stack_pos-1].meta.name, "color_released") == 0)
    {

        key* k = get_key(m_parser_key, m_parser_layer);
        if(k == nullptr)
            return;

        PRINT("color_released: %s\n", jsp->data.str.buff);

        color_t color;
        bool success = am_color_parse(jsp->data.str.buff, &color);
        k->set_color_released(color);
        k->set_parse_error(!success);

        return;
    }


    /* Check for key color_released (RGB Object) */
    if (jsp->stack_pos >=2 && type == LWJSON_STREAM_TYPE_NUMBER && strcmp(jsp->stack[c_stack_pos_key_arg].meta.name, "color_released") == 0)
    {
        PRINT("rgb [%d]: %s\n", jsp->stack[jsp->stack_pos-1].meta.index, jsp->data.str.buff);

        key* k = get_key(m_parser_key, m_parser_layer);
        if(k == nullptr)
            return;

        if(jsp->stack[jsp->stack_pos-1].meta.index == 0)
            k->set_color_released_r(atoi(jsp->data.str.buff));
        else if(jsp->stack[jsp->stack_pos-1].meta.index == 1)
            k->set_color_released_g(atoi(jsp->data.str.buff));
        else if(jsp->stack[jsp->stack_pos-1].meta.index == 2)
            k->set_color_released_b(atoi(jsp->data.str.buff));

        return;
    }



}
