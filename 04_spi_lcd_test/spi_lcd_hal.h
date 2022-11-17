/**
 * @file spi_lcd_hal.h
 *
 */
#ifndef SPI_LCD_HAL
#define SPI_LCD_HAL

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
enum colors565
{
    C_MAROON                   = 0x8000,
    C_DARK_RED                 = 0x8800,
    C_BROWN                    = 0xA145,
    C_FIREBRICK                = 0xB104,
    C_CRIMSON                  = 0xD8A7,
    C_RED                      = 0xF800,
    C_TOMATO                   = 0xFB09,
    C_CORAL                    = 0xFBEA,
    C_INDIAN_RED               = 0xCAEB,
    C_LIGHT_CORAL              = 0xEC10,
    C_DARK_SALMON              = 0xE4AF,
    C_SALMON                   = 0xF40E,
    C_LIGHT_SALMON             = 0xFD0F,
    C_ORANGE_RED               = 0xFA20,
    C_DARK_ORANGE              = 0xFC60,
    C_ORANGE                   = 0xFD20,
    C_GOLD                     = 0xFEA0,
    C_DARK_GOLDEN_ROD          = 0xB421,
    C_GOLDEN_ROD               = 0xDD24,
    C_PALE_GOLDEN_ROD          = 0xEF35,
    C_DARK_KHAKI               = 0xBDAD,
    C_KHAKI                    = 0xEF31,
    C_OLIVE                    = 0x8400,
    C_YELLOW                   = 0xFFE0,
    C_YELLOW_GREEN             = 0x9E66,
    C_DARK_OLIVE_GREEN         = 0x5346,
    C_OLIVE_DRAB               = 0x6C64,
    C_LAWN_GREEN               = 0x7FC0,
    C_CHART_REUSE              = 0x7FE0,
    C_GREEN_YELLOW             = 0xAFE6,
    C_DARK_GREEN               = 0x0320,
    C_GREEN                    = 0x07E0,
    C_FOREST_GREEN             = 0x2444,
    C_LIME                     = 0x07E0,
    C_LIME_GREEN               = 0x3666,
    C_LIGHT_GREEN              = 0x9772,
    C_PALE_GREEN               = 0x97D2,
    C_DARK_SEA_GREEN           = 0x8DD1,
    C_MEDIUM_SPRING_GREEN      = 0x07D3,
    C_SPRING_GREEN             = 0x07EF,
    C_SEA_GREEN                = 0x344B,
    C_MEDIUM_AQUA_MARINE       = 0x6675,
    C_MEDIUM_SEA_GREEN         = 0x3D8E,
    C_LIGHT_SEA_GREEN          = 0x2595,
    C_DARK_SLATE_GRAY          = 0x328A,
    C_TEAL                     = 0x0410,
    C_DARK_CYAN                = 0x0451,
    C_AQUA                     = 0x07FF,
    C_CYAN                     = 0x07FF,
    C_LIGHT_CYAN               = 0xDFFF,
    C_DARK_TURQUOISE           = 0x0679,
    C_TURQUOISE                = 0x46F9,
    C_MEDIUM_TURQUOISE         = 0x4E99,
    C_PALE_TURQUOISE           = 0xAF7D,
    C_AQUA_MARINE              = 0x7FFA,
    C_POWDER_BLUE              = 0xAEFC,
    C_CADET_BLUE               = 0x64F3,
    C_STEEL_BLUE               = 0x4C16,
    C_CORN_FLOWER_BLUE         = 0x64BD,
    C_DEEP_SKY_BLUE            = 0x05FF,
    C_DODGER_BLUE              = 0x249F,
    C_LIGHT_BLUE               = 0xAEBC,
    C_SKY_BLUE                 = 0x867D,
    C_LIGHT_SKY_BLUE           = 0x867E,
    C_MIDNIGHT_BLUE            = 0x18CE,
    C_NAVY                     = 0x0010,
    C_DARK_BLUE                = 0x0011,
    C_MEDIUM_BLUE              = 0x0019,
    C_BLUE                     = 0x001F,
    C_ROYAL_BLUE               = 0x435B,
    C_BLUE_VIOLET              = 0x897B,
    C_INDIGO                   = 0x4810,
    C_DARK_SLATE_BLUE          = 0x49F1,
    C_SLATE_BLUE               = 0x6AD9,
    C_MEDIUM_SLATE_BLUE        = 0x7B5D,
    C_MEDIUM_PURPLE            = 0x939B,
    C_DARK_MAGENTA             = 0x8811,
    C_DARK_VIOLET              = 0x901A,
    C_DARK_ORCHID              = 0x9999,
    C_MEDIUM_ORCHID            = 0xBABA,
    C_PURPLE                   = 0x8010,
    C_THISTLE                  = 0xD5FA,
    C_PLUM                     = 0xDD1B,
    C_VIOLET                   = 0xEC1D,
    C_MAGENTA                  = 0xF81F,
    C_ORCHID                   = 0xDB9A,
    C_MEDIUM_VIOLET_RED        = 0xC0B0,
    C_PALE_VIOLET_RED          = 0xDB92,
    C_DEEP_PINK                = 0xF8B2,
    C_HOT_PINK                 = 0xFB56,
    C_LIGHT_PINK               = 0xFDB7,
    C_PINK                     = 0xFDF9,
    C_ANTIQUE_WHITE            = 0xF75A,
    C_BEIGE                    = 0xF7BB,
    C_BISQUE                   = 0xFF18,
    C_BLANCHED_ALMOND          = 0xFF59,
    C_WHEAT                    = 0xF6F6,
    C_CORN_SILK                = 0xFFBB,
    C_LEMON_CHIFFON            = 0xFFD9,
    C_LIGHT_GOLDEN_ROD_YELLOW  = 0xF7DA,
    C_LIGHT_YELLOW             = 0xFFFB,
    C_SADDLE_BROWN             = 0x8A22,
    C_SIENNA                   = 0x9A85,
    C_CHOCOLATE                = 0xD344,
    C_PERU                     = 0xCC28,
    C_SANDY_BROWN              = 0xF52C,
    C_BURLY_WOOD               = 0xDDB0,
    C_TAN                      = 0xD591,
    C_ROSY_BROWN               = 0xBC71,
    C_MOCCASIN                 = 0xFF16,
    C_NAVAJO_WHITE             = 0xFEF5,
    C_PEACH_PUFF               = 0xFED6,
    C_MISTY_ROSE               = 0xFF1B,
    C_LAVENDER_BLUSH           = 0xFF7E,
    C_LINEN                    = 0xF77C,
    C_OLD_LACE                 = 0xFFBC,
    C_PAPAYA_WHIP              = 0xFF7A,
    C_SEA_SHELL                = 0xFFBD,
    C_MINT_CREAM               = 0xF7FE,
    C_SLATE_GRAY               = 0x7412,
    C_LIGHT_SLATE_GRAY         = 0x7453,
    C_LIGHT_STEEL_BLUE         = 0xAE1B,
    C_LAVENDER                 = 0xE73E,
    C_FLORAL_WHITE             = 0xFFDD,
    C_ALICE_BLUE               = 0xEFBF,
    C_GHOST_WHITE              = 0xF7BF,
    C_HONEYDEW                 = 0xEFFD,
    C_IVORY                    = 0xFFFD,
    C_AZURE                    = 0xEFFF,
    C_SNOW                     = 0xFFDE,
    C_BLACK                    = 0x0000,
    C_DIM_GRAY                 = 0x6B4D,
    C_GRAY                     = 0x8410,
    C_DARK_GRAY                = 0xAD55,
    C_SILVER                   = 0xBDF7,
    C_LIGHT_GRAY               = 0xD69A,
    C_GAINSBORO                = 0xDEDB,
    C_WHITE_SMOKE              = 0xF7BE,
    C_WHITE                    = 0xFFFF,
    // C_TRANSPARENT              = -1,
    C_TRANSPARENT              = C_MAGENTA,
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/
void tft_lcd_100ask_driver_init(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*SPI_LCD_HAL*/