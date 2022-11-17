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

#include "epd_240x360_driver.h"
#include "spi_epd_hal.h"
#include "epd_240x360_luts.h"

/*********************
 *      DEFINES
 *********************/
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void epd_initialize(hal_spi_master_port_t prot);

static void epd_send_cmd(hal_spi_master_port_t prot, uint8_t cmd);
static void epd_send_cmd_data(hal_spi_master_port_t prot, uint8_t cmd, const void *data, uint32_t data_len);
static void epd_send_data(hal_spi_master_port_t prot, const void *data, uint32_t data_len);

static void epd_download_lut5S(void);
static void epd_download_lutGC(void);
static void epd_download_lutDU(void);
static void epd_chkstatus(void);


#define EPD_CMD(port, cmd, data...) {const uint8_t x[] = data; epd_send_cmd_data(port, cmd, x, sizeof(x));}


/**********************
 *  STATIC VARIABLES
 **********************/
static rt_device_t fd;
static hal_spi_master_port_t  spi_port;
static gpio_pin_t pin_busy;
static gpio_pin_t pin_reset;
static gpio_pin_t pin_dc;
static uint16_t display_width;
static uint16_t display_height;

static int g_lut_flag;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int msleep(unsigned int msecs);
static hal_spi_master_config_t cfg;

void epd_240x360_init(epd_driver_t *driver)
{
    cfg.clock_frequency = 25*1000*1000;
    cfg.slave_port = HAL_SPI_MASTER_SLAVE_0;
    cfg.cpha = HAL_SPI_MASTER_CLOCK_PHASE0;
    cfg.cpol = HAL_SPI_MASTER_CLOCK_POLARITY0;
    
    display_width = driver->width;
    display_height = driver->height;
    pin_busy = driver->pin_busy;
    pin_reset = driver->pin_reset;
    pin_dc = driver->pin_dc;
    
    //hal_gpio_set_pull(pin_busy, GPIO_PULL_UP);
    //hal_gpio_set_driving_level(pin_reset, GPIO_DRIVING_LEVEL3);
    //hal_gpio_set_driving_level(pin_dc, GPIO_DRIVING_LEVEL3);
    hal_gpio_set_direction(pin_busy, GPIO_DIRECTION_INPUT);
    hal_gpio_set_direction(pin_reset, GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(pin_dc, GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_data(pin_reset, GPIO_DATA_HIGH);
    

    spi_port = (hal_spi_master_port_t)driver->spi_host;
    hal_spi_init(spi_port, &cfg);
    //fd = rt_device_find("spi1");
    //rt_device_open(fd, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM);
    //rt_device_control(fd, SPI_CONFIG, &cfg);
    
    epd_initialize(spi_port);
}

void epd_240x360_deinit(void)
{
    hal_spi_deinit(spi_port);
    //rt_device_close((rt_device_t)fd);
}

void epd_240x360_reset(void)
{
    epd_initialize(spi_port);
}


void epd_240x360_display_clear(uint8_t color)
{
    unsigned int row, column;
    
    epd_send_cmd(spi_port, 0x10);
    for (column = 0; column < display_height; column++)
    {
        for (row = 0; row < display_width / 8; row++)
        {
            epd_send_data(spi_port, &color, 1);
        }
    }

    epd_send_cmd(spi_port, 0x13);
    for (column = 0; column < display_height; column++)
    {
        for (row = 0; row < display_width / 8; row++)
        {
            epd_send_data(spi_port, &color, 1);
        }
    }
}


void epd_240x360_display_img(const uint8_t * img, uint16_t w, uint16_t h)
{
    if(w > display_width)     w = display_width;
    if(h > display_height)    h = display_height;

    epd_send_cmd(spi_port, 0x13);
    epd_send_data(spi_port, img, ((w * h) / 8));
}


// partial display 
void epd_240x360_display_partial(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint8_t * data)
{
    uint16_t x_start1, x_end1, y_start1, y_start2, y_end1, y_end2;
    uint8_t tmp_data[8];

    x_start1 = x0;//转换为字节
    x_end1 = x0 + h-1; 

    y_start1 = 0;
    y_start2 = y0;
    if(y0 >= 256)
    {
        y_start1 = y_start2 / 256;
        y_start2 = y_start2 % 256;
    }

    y_end1 = 0;
    y_end2 = y0 + w - 1;
    if(y_end2 >= 256)
    {
        y_end1 = y_end2 / 256;
        y_end2 = y_end2 % 256;		
    }

    /*********************************************************/	
    //需要重新复位和初始化设置!!!!
    /*********************************************************/	
    epd_240x360_reset();

    tmp_data[0] = 0xF7;                 // Border
    epd_send_cmd(spi_port, 0x50);
    epd_send_data(spi_port, tmp_data, 1);

    /*********************************************************/		
    tmp_data[0] = 0xFF;                 // RES1 RES0 REG KW/R     UD    SHL   SHD_N  RST_N	
    tmp_data[1] = 0x01;                 // x x x VCMZ TS_AUTO TIGE NORG VC_LUTZ	
    epd_send_cmd(spi_port, 0x00);          // panel setting   PSR
    epd_send_data(spi_port, tmp_data, 2);

    tmp_data[0] = x_start1;             //x-start
    tmp_data[1] = x_end1;               //x-end
    tmp_data[2] = y_start1;
    tmp_data[3] = y_start2;             //y-start 
    tmp_data[4] = y_end1;
    tmp_data[5] = y_end2;               //y-end
    tmp_data[6] = 0x01;
    epd_send_cmd(spi_port, 0x91);          //This command makes the display enter partial mode
    epd_send_cmd(spi_port, 0x90);          //resolution setting
    epd_send_data(spi_port, tmp_data, 7);  

    epd_send_cmd(spi_port, 0x13);          // writes New data to SRAM.
    epd_send_data(spi_port, data, ((w * h) / 8));      // show data(image)
}


void epd_240x360_refresh(EPD_LUT_TYPE lut)
{
    uint8_t tmp_data[1];
    switch (lut)
    {
    case EPD_LUT_GC:
        epd_download_lutGC();

        //DISPLAY REFRESH
        tmp_data[0] = 0xA5;
        epd_send_cmd(spi_port, 0x17);
        epd_send_data(spi_port, tmp_data, 1);

        epd_chkstatus();
        msleep(30);

        break;

    case EPD_LUT_DU:
        epd_download_lutDU();

        //DISPLAY REFRESH
        tmp_data[0] = 0xA5;
        epd_send_cmd(spi_port, 0x17);
        epd_send_data(spi_port, tmp_data, 1);
        
        epd_chkstatus();
        msleep(40);

        epd_240x360_reset();	

        tmp_data[0] = 0xD7;  // Border
        epd_send_cmd(spi_port, 0x50);
        epd_send_data(spi_port, tmp_data, 1);

        break;

    case EPD_LUT_5S:
        epd_download_lut5S();
        break;

    default:
        return;
        break;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
//Initialize the display
static void epd_initialize(hal_spi_master_port_t prot)
{
    /*Reset the display*/
    hal_gpio_set_data(pin_reset, GPIO_DATA_HIGH);
    msleep(20);
    hal_gpio_set_data(pin_reset, GPIO_DATA_LOW);
    msleep(20);
    hal_gpio_set_data(pin_reset, GPIO_DATA_HIGH);
    msleep(20);

    /*init epd*/
    /* panel setting   PSR 
     * RES1 RES0 REG KW/R     UD    SHL   SHD_N  RST_N
     * x x x VCMZ TS_AUTO TIGE NORG VC_LUTZ
     */
    EPD_CMD(prot, 0x00, {0xFF,0x01});
    /* POWER SETTING   PWR
     * cp1 keeps 1 frame, 1st frame enable
     * x x x VCOM_SLWE VGH[3:0]   VGH=20V, VGL=-20V
     * x x VSH[5:0]  VSH = 15V
     * x x VSL[5:0]  VSL=-15V
     * OPTEN VDHR[6:0]  VHDR=6.4V
     */
	EPD_CMD(prot, 0x01, {0x03, 0x10, 0x3f, 0x3f, 0x03});
    /* T_VDS_OFF[1:0] 00=1 frame; 01=2 frame; 10=3 frame; 11=4 frame
     * booster soft start   BTST
     * BT_PHA[7:0]
     * BT_PHB[7:0]
     * x x BT_PHC[5:0]
     */
	EPD_CMD(prot, 0x06, {0x17, 0x37, 0x3D});
    /* VCOM_DC setting    VDCS
        * x  VDCS[6:0] VCOM_DC value= -1.9v    00~3f,0x12=-1.9v
        */
	EPD_CMD(prot, 0x82, {0x07});
	EPD_CMD(prot, 0x30, {0x09});
    /* resoultion setting
     * HRES[7:3] 0 0 0
     * x x x x x x x VRES[8]
     * VRES[7:0]
     */
    EPD_CMD(prot, 0x61, {0xf0, 0x01, 0x68});
    EPD_CMD(prot, 0x50, {0XD7});
}

static void epd_send_cmd(hal_spi_master_port_t prot, uint8_t cmd)
{
    hal_gpio_set_data(pin_dc, GPIO_DATA_LOW);
    hal_spi_write(prot, &cmd, 1);
    //rt_device_write((rt_device_t)fd, 0, &cmd, 1);
}

static void epd_send_data(hal_spi_master_port_t prot, const void *data, uint32_t data_len)
{
    //if (data && data_len > 0)
    {
        hal_gpio_set_data(pin_dc, GPIO_DATA_HIGH);
        hal_spi_write(prot, data, data_len);
        //rt_device_write((rt_device_t)fd, 0, data, data_len);
    }
}

static void epd_send_cmd_data(hal_spi_master_port_t prot, uint8_t cmd, const void *data, uint32_t data_len)
{
    hal_gpio_set_data(pin_dc, GPIO_DATA_LOW);
    hal_spi_write(prot, &cmd, 1);
    //rt_device_write((rt_device_t)fd, 0, &cmd, 1);
    //if (data && data_len > 0)
    {
        hal_gpio_set_data(pin_dc, GPIO_DATA_HIGH);
        hal_spi_write(prot, data, data_len);
        //rt_device_write((rt_device_t)fd, 0, data, data_len);
    }
}


static void epd_chkstatus(void)
{
    gpio_data_t gpio_data;
    
    do{
        hal_gpio_get_data(pin_busy, &gpio_data);
        //printf("data :0x%0x\n", gpio_data);
        msleep(5);
    }while (gpio_data == 0);
        
}



/*****************************LUT download*************************/
static void epd_download_lut5S(void)
{				
  //vcom
  epd_send_cmd(spi_port, 0x20);
  epd_send_data(spi_port, lut_vcom, 42);
						
  //red not use
  epd_send_cmd(spi_port, 0x21);
  epd_send_data(spi_port, lut_ww, 42);
				
  //wb w
  epd_send_cmd(spi_port, 0x24);
  epd_send_data(spi_port, lut_bb, 42);

  if (g_lut_flag == 0)
  {						
    //bb b
    epd_send_cmd(spi_port, 0x22);
    epd_send_data(spi_port, lut_bw, 42);
					
    //bw r
    epd_send_cmd(spi_port, 0x23);
    epd_send_data(spi_port, lut_wb, 42);


    g_lut_flag = 1;
  }
  else
  {
    //bb b
    epd_send_cmd(spi_port, 0x23);
    epd_send_data(spi_port, lut_bw, 42);
					
    //bw r
    epd_send_cmd(spi_port, 0x22);
    epd_send_data(spi_port, lut_wb, 42);

    g_lut_flag = 0;
  }
}


//LUT download
static void epd_download_lutGC(void)
{
  epd_send_cmd(spi_port, 0x20);
  epd_send_data(spi_port, lut_R20_GC, 56);

  //red not use
  epd_send_cmd(spi_port, 0x21);
  epd_send_data(spi_port, lut_R21_GC, 42);				

  //bb b
  epd_send_cmd(spi_port, 0x24);
  epd_send_data(spi_port, lut_R24_GC, 42);	

  if (g_lut_flag == 0)	
  {						
    //bw r
    epd_send_cmd(spi_port, 0x22);
    epd_send_data(spi_port, lut_R22_GC, 56);	
					
    //wb w
    epd_send_cmd(spi_port, 0x23);
    epd_send_data(spi_port, lut_R23_GC, 42);
    g_lut_flag = 1;
  }
  else
  {						
    //bw r
    epd_send_cmd(spi_port, 0x22);
    epd_send_data(spi_port, lut_R23_GC, 42);	

    //wb w
    epd_send_cmd(spi_port, 0x23);
    epd_send_data(spi_port, lut_R22_GC, 56);
    g_lut_flag = 0;

  }

}



//LUT download
static void epd_download_lutDU(void)
{		
    //vcom
    epd_send_cmd(spi_port, 0x20);
    epd_send_data(spi_port, lut_R20_DU, 56);	
		
    //red not use
    epd_send_cmd(spi_port, 0x21);
    epd_send_data(spi_port, lut_R21_DU, 42);	
						
    //bb b
    epd_send_cmd(spi_port, 0x24);
    epd_send_data(spi_port, lut_R24_DU, 42);	

    if (g_lut_flag == 0)	
    {
      //bw r
      epd_send_cmd(spi_port, 0x22);
      epd_send_data(spi_port, lut_R22_DU, 56);							

      //wb w
      epd_send_cmd(spi_port, 0x23);
      epd_send_data(spi_port, lut_R23_DU, 42);
      g_lut_flag = 1;
    }
    else
    {
      //bw r
      epd_send_cmd(spi_port, 0x22);
      epd_send_data(spi_port, lut_R23_DU, 42);							

      //wb w
      epd_send_cmd(spi_port, 0x23);
      epd_send_data(spi_port, lut_R22_DU, 56);
      g_lut_flag = 0;
    }
}
