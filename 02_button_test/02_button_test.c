#include <hal/hal_gpio.h>
#include <drv/sunxi_drv_gpio.h>
#include <stdio.h>
#include <rtthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TEST_BUTTON_100ASK_PIN  GPIO_PE7
#define TEST_LED_100ASK_PIN     GPIO_PE6

int test_button_100ask(int argc, char **argv)
{
    gpio_pull_status_t pull_state;
    gpio_direction_t gpio_direction;
    gpio_data_t gpio_data;

    hal_gpio_set_pull(TEST_BUTTON_100ASK_PIN, GPIO_PULL_UP);
    hal_gpio_set_direction(TEST_BUTTON_100ASK_PIN, GPIO_DIRECTION_INPUT);

    hal_gpio_set_pull(TEST_LED_100ASK_PIN, GPIO_PULL_UP);
    hal_gpio_set_direction(TEST_LED_100ASK_PIN, GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_data(TEST_LED_100ASK_PIN, GPIO_DATA_LOW);

    hal_gpio_get_pull(TEST_BUTTON_100ASK_PIN, &pull_state);
    hal_gpio_get_direction(TEST_BUTTON_100ASK_PIN, &gpio_direction);
    hal_gpio_get_data(TEST_BUTTON_100ASK_PIN, &gpio_data);
    printf("Button pin pull state : %d, dir : %d, data :0x%0x\n", pull_state, gpio_direction, gpio_data);

    int msleep(unsigned int msecs);
    while(1)
    {
        hal_gpio_get_data(TEST_BUTTON_100ASK_PIN, &gpio_data);
        
        if(gpio_data == 0)  hal_gpio_set_data(TEST_LED_100ASK_PIN, GPIO_DATA_HIGH);
        else    hal_gpio_set_data(TEST_LED_100ASK_PIN, GPIO_DATA_LOW);
        
        msleep(30);
    }

    return 0;
}

FINSH_FUNCTION_EXPORT_ALIAS(test_button_100ask, __cmd_100ask_test_button, Code to monitor button status);


int test_button_switch_100ask(int argc, char **argv)
{
    bool button_state = false;
    gpio_pull_status_t pull_state;
    gpio_direction_t gpio_direction;
    gpio_data_t gpio_data;

    hal_gpio_set_pull(TEST_BUTTON_100ASK_PIN, GPIO_PULL_UP);
    hal_gpio_set_direction(TEST_BUTTON_100ASK_PIN, GPIO_DIRECTION_INPUT);

    hal_gpio_set_pull(TEST_LED_100ASK_PIN, GPIO_PULL_UP);
    hal_gpio_set_direction(TEST_LED_100ASK_PIN, GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_data(TEST_LED_100ASK_PIN, GPIO_DATA_LOW);

    hal_gpio_get_pull(TEST_BUTTON_100ASK_PIN, &pull_state);
    hal_gpio_get_direction(TEST_BUTTON_100ASK_PIN, &gpio_direction);
    hal_gpio_get_data(TEST_BUTTON_100ASK_PIN, &gpio_data);
    printf("Button pin pull state : %d, dir : %d, data :0x%0x\n", pull_state, gpio_direction, gpio_data);

    int msleep(unsigned int msecs);
    while(1)
    {
        hal_gpio_get_data(TEST_BUTTON_100ASK_PIN, &gpio_data);
        if(gpio_data == 0)
        {
            msleep(50);
            hal_gpio_get_data(TEST_BUTTON_100ASK_PIN, &gpio_data);
            if(gpio_data == 0)  button_state = !button_state;
        }

        if(button_state)    hal_gpio_set_data(TEST_LED_100ASK_PIN, GPIO_DATA_HIGH);
        else                hal_gpio_set_data(TEST_LED_100ASK_PIN, GPIO_DATA_LOW);
        
        //msleep(30);
    }

    return 0;
}

FINSH_FUNCTION_EXPORT_ALIAS(test_button_switch_100ask, __cmd_100ask_test_button_switch, Code to Control LED light through button(switch));