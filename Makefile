subdir-ccflags-y :=  -I$(srctree)/ekernel/core/rt-thread/include

obj-${CONFIG_100ASK_LED_TEST} += 01_led_test/
obj-${CONFIG_100ASK_BUTTON_TEST} += 02_button_test/
obj-${CONFIG_100ASK_FC_JOYPAD_TEST} += 03_fc_joypad_test/
obj-${CONFIG_100ASK_SPI_LCD_TEST} += 04_spi_lcd_test/
obj-${CONFIG_100ASK_SPI_EPD_TEST} += 05_spi_epd_test/





