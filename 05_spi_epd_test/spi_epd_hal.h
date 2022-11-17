/**
 * @file spi_epd_hal.h
 *
 */
#ifndef SPI_EPD_HAL
#define SPI_EPD_HAL

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/
#define EPD_100ASK_COLOR_WHITE     0xFF
#define EPD_100ASK_COLOR_BLACK     0x00

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void epd_100ask_driver_init(void);

void epd_100ask_driver_deinit(void);

void epd_100ask_display_clear(uint8_t color);

void epd_100ask_display_img(const uint8_t * img, uint16_t w, uint16_t h);

void epd_100ask_display_partial(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint8_t * data);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*SPI_EPD_HAL*/