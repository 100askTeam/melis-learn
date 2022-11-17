/**
 * @file st7796.h
 *
 */
#ifndef ST7796S_H
#define ST7796S_H

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
typedef struct ST7796S_driver {
    int pin_backlight;
	int pin_reset;
	int pin_dc;
	int spi_host;
	uint16_t display_width;
	uint16_t display_height;
} ST7796S_driver_t;

typedef struct ST7796S_command {
	uint8_t command;
	uint8_t wait_ms;
	uint8_t data_size;
	const uint8_t *data;
} ST7796S_command_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void st7796s_init(ST7796S_driver_t *driver);

void st7796s_display_clear(uint16_t color_le);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*ST7796S_H*/