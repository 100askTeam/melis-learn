/**
 * @file spi_lcd_hal.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <hal/hal_gpio.h>
#include <drv/sunxi_drv_gpio.h>
#include <sunxi_hal_spi.h>
#include <sunxi_drv_spi.h>

#include "spi_lcd_hal.h"
#include "st7796s.h"

/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/


/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static ST7796S_driver_t display = {
    .pin_backlight  = GPIO_PG1,
    .pin_reset      = GPIO_PG2,
    .pin_dc         = GPIO_PG4,
    .spi_host       = 1,
    .display_width  = 320,
    .display_height = 480,
};


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void tft_lcd_100ask_driver_init(void)
{
    st7796s_init(&display);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

