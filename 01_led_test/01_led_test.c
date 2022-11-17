#include <hal/hal_gpio.h>
#include <drv/sunxi_drv_gpio.h>
#include <stdio.h>
#include <rtthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TEST_LED_100ASK_PIN GPIO_PE6

int ctrl_led_100ask(int argc, char **argv)
{
    uint32_t irq;
    int set_gpio_data = 0;
    gpio_pull_status_t pull_state;
    gpio_direction_t gpio_direction;
    gpio_data_t gpio_data;

	if(argc < 2)
	{
		printf("uasge : 100ask_ctrl_led [10], eg:100ask_ctrl_led 1 \n");
		return 0;
	}

    set_gpio_data = atoi(argv[1]);

    hal_gpio_get_pull(TEST_LED_100ASK_PIN, &pull_state);
    hal_gpio_get_direction(TEST_LED_100ASK_PIN, &gpio_direction);
    hal_gpio_get_data(TEST_LED_100ASK_PIN, &gpio_data);
    printf("pull state : %d, dir : %d, data :0x%0x\n", pull_state, gpio_direction, gpio_data);

    hal_gpio_set_pull(TEST_LED_100ASK_PIN, GPIO_PULL_UP);
    hal_gpio_set_direction(TEST_LED_100ASK_PIN, GPIO_DIRECTION_OUTPUT);
    //hal_gpio_set_data(TEST_LED_100ASK_PIN, GPIO_DATA_HIGH);
    hal_gpio_set_data(TEST_LED_100ASK_PIN, set_gpio_data);
    //hal_gpio_sel_vol_mode(TEST_LED_100ASK_PIN, POWER_MODE_180);

    hal_gpio_get_pull(TEST_LED_100ASK_PIN, &pull_state);
    hal_gpio_get_direction(TEST_LED_100ASK_PIN, &gpio_direction);
    hal_gpio_get_data(TEST_LED_100ASK_PIN, &gpio_data);

    printf("pull state : %d, dir : %d, data :0x%0x\n", pull_state, gpio_direction, gpio_data);

    return 0;
}

FINSH_FUNCTION_EXPORT_ALIAS(ctrl_led_100ask, __cmd_100ask_ctrl_led, Code to control the LED light on/off);

int blink_led_100ask(int argc, char **argv)
{
    int delay_time = 500;
    gpio_pull_status_t pull_state;
    gpio_direction_t gpio_direction;
    gpio_data_t gpio_data;

    if(argc < 2)
	{
		printf("uasge : 100ask_blink_led [time(ms)], eg:100ask_blink_led 500 \n");
		return 0;
	}

    delay_time = atoi(argv[1]);

    hal_gpio_get_pull(TEST_LED_100ASK_PIN, &pull_state);
    hal_gpio_get_direction(TEST_LED_100ASK_PIN, &gpio_direction);
    hal_gpio_get_data(TEST_LED_100ASK_PIN, &gpio_data);
    printf("before pull state : %d, dir : %d, data :0x%0x\n", pull_state, gpio_direction, gpio_data);

    hal_gpio_set_driving_level(TEST_LED_100ASK_PIN, GPIO_DRIVING_LEVEL3);
    hal_gpio_set_direction(TEST_LED_100ASK_PIN, GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_data(TEST_LED_100ASK_PIN, GPIO_DATA_HIGH);
    hal_gpio_sel_vol_mode(TEST_LED_100ASK_PIN, POWER_MODE_330);
    hal_gpio_set_debounce(TEST_LED_100ASK_PIN, 1);

    for(int i=0; i<=10; i++)
    {
        int msleep(unsigned int msecs);
        msleep(delay_time);
        hal_gpio_set_data(TEST_LED_100ASK_PIN, GPIO_DATA_HIGH);
        msleep(delay_time);
        hal_gpio_set_data(TEST_LED_100ASK_PIN, GPIO_DATA_LOW);
    }
    
    hal_gpio_get_pull(TEST_LED_100ASK_PIN, &pull_state);
    hal_gpio_get_direction(TEST_LED_100ASK_PIN, &gpio_direction);
    hal_gpio_get_data(TEST_LED_100ASK_PIN, &gpio_data);

    printf("after pull state : %d, dir : %d, data :0x%0x\n", pull_state, gpio_direction, gpio_data);

    return 0;
}

FINSH_FUNCTION_EXPORT_ALIAS(blink_led_100ask, __cmd_100ask_test_blink_led, Code to blink LED);


