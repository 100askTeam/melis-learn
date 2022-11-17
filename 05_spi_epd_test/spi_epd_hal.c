/**
 * @file spi_epd_hal.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <hal/hal_gpio.h>
#include <drv/sunxi_drv_gpio.h>
#include <sunxi_hal_spi.h>
#include <sunxi_drv_spi.h>

#include "spi_epd_hal.h"
#include "epd_240x360_driver.h"

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
static epd_driver_t display = {
    .pin_busy       = GPIO_PG1,
    .pin_reset      = GPIO_PG2,
    .pin_dc         = GPIO_PG4,
    .spi_host       = 1,
    .width          = 240,
    .height         = 360,
};


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void epd_100ask_driver_init(void)
{
    epd_240x360_init(&display);
}

void epd_100ask_driver_deinit(void)
{
    epd_240x360_deinit();
}

void epd_100ask_display_clear(uint8_t color)
{
    epd_240x360_display_clear(color);
}


void epd_100ask_display_img(const uint8_t * img, uint16_t w, uint16_t h)
{
    epd_240x360_display_img(img, w, h);
}


void epd_100ask_display_partial(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint8_t * data)
{
    epd_240x360_display_partial(x0, y0, w, h, data);
    epd_240x360_refresh(EPD_LUT_DU);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

