/**
 * @file st7796s.c
 *
 */
/*********************
 *      INCLUDES
 *********************/
#include <getopt.h>
#include <log.h>
#include <rtthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <rtthread.h>
#include <sunxi_hal_spi.h>
#include <sunxi_drv_spi.h>

#include "st7796s.h"
#include "spi_lcd_hal.h"

/*********************
 *      DEFINES
 *********************/
#define RG_MIN(a, b) ({__typeof__(a) _a = (a); __typeof__(b) _b = (b);_a < _b ? _a : _b; })
#define RG_MAX(a, b) ({__typeof__(a) _a = (a); __typeof__(b) _b = (b);_a > _b ? _a : _b; })
#define RG_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#define SPI_BUFFER_LENGTH     (4 * 480) // In pixels (uint16)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lcd_set_window(int left, int top, int width, int height);
static void st7796s_cmd(uint8_t cmd, const void *data, size_t data_len);

#define ST7796S_CMD(cmd, data...) {const uint8_t x[] = data; st7796s_cmd(cmd, x, sizeof(x));}


/**********************
 *  STATIC VARIABLES
 **********************/
static rt_device_t fd;
static hal_spi_master_port_t  spi_port;
static gpio_pin_t pin_reset;
static gpio_pin_t pin_dc;
static uint16_t display_width;
static uint16_t display_height;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int msleep(unsigned int msecs);

void st7796s_init(ST7796S_driver_t *driver)
{
    hal_spi_master_config_t cfg;

    cfg.clock_frequency = 40*1000*1000;
    cfg.slave_port = HAL_SPI_MASTER_SLAVE_0;
    cfg.cpha = HAL_SPI_MASTER_CLOCK_PHASE0;
    cfg.cpol = HAL_SPI_MASTER_CLOCK_POLARITY0;
    
    display_width = driver->display_width;
    display_height = driver->display_height;
    pin_reset = driver->pin_reset;
    pin_dc = driver->pin_dc;
    hal_gpio_set_direction(driver->pin_reset, GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(driver->pin_dc, GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(driver->pin_backlight, GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_data(driver->pin_backlight, GPIO_DATA_HIGH);
    hal_gpio_set_data(driver->pin_dc, GPIO_DATA_HIGH);
    hal_gpio_set_data(driver->pin_reset, GPIO_DATA_HIGH);

    spi_port = (hal_spi_master_port_t)driver->spi_host;
    hal_spi_init(spi_port, &cfg);
    //fd = rt_device_find("spi1");
    //rt_device_open(fd, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM);
    //rt_device_control(fd, SPI_CONFIG, &cfg);
    
    /*init st7796s*/
    ST7796S_CMD(0x11, {0x00});
    msleep(120);
	ST7796S_CMD(0xF0, {0xC3});
	ST7796S_CMD(0xF0, {0x96});
	ST7796S_CMD(0x36, {0x48});
	ST7796S_CMD(0xB4, {0x01});
    ST7796S_CMD(0xB7, {0xC6});
    ST7796S_CMD(0xE8, {0x40,0X8A,0X00,0X00,0X29,0X19,0XA5,0X33});
    ST7796S_CMD(0xC1, {0x06});
    ST7796S_CMD(0xC2, {0xA7});
    ST7796S_CMD(0xC5, {0x18});
    ST7796S_CMD(0xE0, {0xF0,0x09,0x0B,0x06,0x04,0x15,0x2F,0x54,0x42,0x3C,0x17,0x14,0x18,0x1B});
    ST7796S_CMD(0xE1, {0xf0,0x09,0x0b,0x06,0x04,0x03,0x2d,0x43,0x42,0x3b,0x16,0x14,0x17,0x1b}); //Negative Voltage Gamma Control
    ST7796S_CMD(0xF0, {0x3C});
    ST7796S_CMD(0xF0, {0x69});
    ST7796S_CMD(0x3A, {0x55});
    msleep(120);
    ST7796S_CMD(0x29, {0x00}); //Display ON
    
    ST7796S_CMD(0x2A, {0x00,0x00,((driver->display_width-1)>>8),((driver->display_width-1)&0XFF)});
    ST7796S_CMD(0x2B, {0x00,0x00,((driver->display_height-1)>>8),((driver->display_height-1)&0XFF)});

    st7796s_display_clear(C_RED);
    msleep(1000);
    st7796s_display_clear(C_GREEN);
    msleep(1000);
    st7796s_display_clear(C_BLUE);
    msleep(1000);
    st7796s_display_clear(C_WHITE);
    msleep(1000);

    hal_spi_deinit(spi_port);
    //rt_device_close((rt_device_t)fd);
}


void st7796s_display_clear(uint16_t color_le)
{
    size_t pixels = display_width * display_height;
    uint16_t color = (color_le << 8) | (color_le >> 8);

    // We ignore margins here
    lcd_set_window(0, 0, display_width, display_height);

    uint16_t buffer[SPI_BUFFER_LENGTH];
    hal_gpio_set_data(pin_dc, GPIO_DATA_HIGH);
    while (pixels > 0)
    {
        size_t count = RG_MIN(pixels, SPI_BUFFER_LENGTH);

        for (size_t j = 0; j < count; ++j)
            buffer[j] = color;
        hal_spi_write(spi_port, buffer, count * 2);
        //rt_device_write((rt_device_t)fd, 0, buffer, count * 2);
        pixels -= count;

        //rt_device_write((rt_device_t)fd, 0, &color, 2);
        //pixels--;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void lcd_set_window(int left, int top, int width, int height)
{
    int right = left + width - 1;
    int bottom = top + height - 1;

    if (left < 0 || top < 0 || right >= display_width || bottom >= display_height)
    {
        printf("Bad lcd window (x0=%d, y0=%d, x1=%d, y1=%d)\n", left, top, right, bottom);
    }

    st7796s_cmd(0x2A, (uint8_t[]){left >> 8, left & 0xff, right >> 8, right & 0xff}, 4); // Horiz
    st7796s_cmd(0x2B, (uint8_t[]){top >> 8, top & 0xff, bottom >> 8, bottom & 0xff}, 4); // Vert
    st7796s_cmd(0x2C, NULL, 0); // Memory write
}


static void st7796s_cmd(uint8_t cmd, const void *data, size_t data_len)
{
    hal_gpio_set_data(pin_dc, GPIO_DATA_LOW);
    hal_spi_write(spi_port, &cmd, 1);
    //rt_device_write((rt_device_t)fd, 0, &cmd, 1);
    if (data && data_len > 0)
    {
        hal_gpio_set_data(pin_dc, GPIO_DATA_HIGH);
        hal_spi_write(spi_port, data, data_len);
        //rt_device_write((rt_device_t)fd, 0, data, data_len);
    }
}