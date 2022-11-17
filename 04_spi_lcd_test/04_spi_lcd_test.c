#include <rtthread.h>

#include "spi_lcd_hal.h"



static int spi_lcd_test_100ask(int argc, const char **argv)
{
    tft_lcd_100ask_driver_init();

    return 0;
}

FINSH_FUNCTION_EXPORT_ALIAS(spi_lcd_test_100ask, __cmd_100ask_test_spi_lcd, Code to spi lcd test);
