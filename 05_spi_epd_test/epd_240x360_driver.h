/**
 * @file epd_240x360.h
 *
 */
#ifndef EPD_240X360_DRIVER_H
#define EPD_240X360_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct epd_driver {
    int pin_busy;
	int pin_reset;
	int pin_dc;
	int spi_host;
	uint16_t width;
	uint16_t height;
} epd_driver_t;

/**
 * Refresh type
**/
typedef enum {
    EPD_LUT_GC,         /* 全刷 */
    EPD_LUT_DU,         /* 局刷 */
    EPD_LUT_5S,         /* ... */
    _EPD_LUT_LAST,      /** Number of default events*/
} EPD_LUT_TYPE;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void epd_240x360_init(epd_driver_t *driver);

void epd_240x360_deinit(void);

void epd_240x360_reset(void);

void epd_240x360_display_clear(uint8_t color);

void epd_240x360_display_img(const uint8_t * img, uint16_t w, uint16_t h);

void epd_240x360_display_partial(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint8_t * data);

void epd_240x360_refresh(EPD_LUT_TYPE lut);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*EPD_240X360_DRIVER_H*/