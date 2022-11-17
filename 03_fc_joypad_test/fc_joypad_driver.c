/**
 * @file fc_joypad_driver.c
 *
 */
/*
 ***************************************
    CLOCK_PIN	----- ID
    LATCH_PIN	----- D+
    DATA_PIN	----- D-
 ***************************************
    A:      0000 0001 1     (1 << 0)
    B:      0000 0010 2     (1 << 1)
    select:	0000 1000 4     (1 << 2)
    start: 	0000 0100 8     (1 << 3)
    Up:     0001 0000 16    (1 << 4)
    Down:   0010 0000 32    (1 << 5)
    Left:   0100 0000 64    (1 << 6)
    Right:  1000 0000 128   (1 << 7)
 ***************************************
*/
/*********************
 *      INCLUDES
 *********************/
#include <hal/hal_gpio.h>
#include <drv/sunxi_drv_gpio.h>
#include <stdio.h>
#include <rtthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "fc_joypad_driver.h"

/*********************
 *      DEFINES
 *********************/
#define FC_JOYPAD_CLOCK_PIN        GPIO_PG12    /*ID*/
#define FC_JOYPAD_LATCH_PIN        GPIO_PG14    /*D+*/
#define FC_JOYPAD_DATA_PIN         GPIO_PG15     /*D-*/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {    
    char prev_player_value;
    char player;
    bool joypad_state;
} fc_joypad_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static fc_joypad_t g_fc_joypad;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int msleep(unsigned int msecs);

void fc_joypad_100ask_init(void)
{    
    hal_gpio_set_direction(FC_JOYPAD_CLOCK_PIN, GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(FC_JOYPAD_LATCH_PIN, GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(FC_JOYPAD_DATA_PIN, GPIO_DIRECTION_INPUT);
}


void fc_joypad_100ask_read(void)
{
    uint8_t count = 0;
    int8_t tmp_value = g_fc_joypad.player;
    gpio_data_t gpio_data;

    hal_gpio_set_data(FC_JOYPAD_LATCH_PIN, GPIO_DATA_HIGH);
    msleep(5); // ms
    hal_gpio_set_data(FC_JOYPAD_LATCH_PIN, GPIO_DATA_LOW);
    msleep(5); // ms

    g_fc_joypad.joypad_state = false;
    g_fc_joypad.player = 0;
    for (count = 0; count < 8; count++)
    {
        hal_gpio_get_data(FC_JOYPAD_DATA_PIN, &gpio_data);
        if (gpio_data == 0)
        {
            if (!(g_fc_joypad.joypad_state))
                g_fc_joypad.prev_player_value = tmp_value;

            g_fc_joypad.player |= (1 << count);
            g_fc_joypad.joypad_state = true;
        }  
        
        hal_gpio_set_data(FC_JOYPAD_CLOCK_PIN, GPIO_DATA_HIGH);
        msleep(5); // ms
        hal_gpio_set_data(FC_JOYPAD_CLOCK_PIN, GPIO_DATA_LOW);
        msleep(5); // ms
    }
}



bool fc_joypad_100ask_is_pressed(fc_joypad_100ask_btn_t btn)
{
    if ((((uint8_t)1 << btn) & g_fc_joypad.player) >= 1)
        return true;
    else
        return false;
}


bool fc_joypad_100ask_is_release(fc_joypad_100ask_btn_t btn)
{
    if ((((uint8_t)1 << btn) & g_fc_joypad.prev_player_value) >= 1)
        return true;
    else
        return false;
}


fc_joypad_100ask_state_t fc_joypad_100ask_state(void)
{
    if(g_fc_joypad.joypad_state)
        return  FC_JOYPAD_100ASK_PRESSED;
    else
        return FC_JOYPAD_100ASK_RELEASE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

