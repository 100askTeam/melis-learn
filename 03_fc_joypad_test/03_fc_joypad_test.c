#include <hal/hal_gpio.h>
#include <drv/sunxi_drv_gpio.h>
#include <stdio.h>
#include <rtthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "fc_joypad_driver.h"

static void show_joypad_state_info(fc_joypad_100ask_btn_t btn, fc_joypad_100ask_state_t btn_state);

int fc_joypad_100ask(int argc, char **argv)
{
    uint8_t count = 0;
    fc_joypad_100ask_state_t btn_state = FC_JOYPAD_100ASK_RELEASE;

    fc_joypad_100ask_init();

    while(1) {
        fc_joypad_100ask_read();
        btn_state = fc_joypad_100ask_state();
        for (count = 0; count < 8; count++)
        {
            if (fc_joypad_100ask_is_pressed((fc_joypad_100ask_btn_t)count))
                show_joypad_state_info((fc_joypad_100ask_btn_t)count, btn_state);
            else if (fc_joypad_100ask_is_release((fc_joypad_100ask_btn_t)count))
                show_joypad_state_info((fc_joypad_100ask_btn_t)count, btn_state);
        }

    }

    return 0;
}


static void show_joypad_state_info(fc_joypad_100ask_btn_t btn, fc_joypad_100ask_state_t btn_state)
{
  switch (btn)
  {
    case FC_JOYPAD_100ASK_BTN_A:
      printf("Key FC_JOYPAD_BTN_A is ");
      break;
    case FC_JOYPAD_100ASK_BTN_B:
      printf("Key FC_JOYPAD_BTN_B is ");
      break;
    case FC_JOYPAD_100ASK_BTN_START:
      printf("Key FC_JOYPAD_BTN_START is ");
      break;
    case FC_JOYPAD_100ASK_BTN_SELECT:
      printf("Key FC_JOYPAD_BTN_SELECT is ");
      break;
    case FC_JOYPAD_100ASK_BTN_UP:
      printf("Key FC_JOYPAD_BTN_UP is ");
      break;
    case FC_JOYPAD_100ASK_BTN_DOWN:
      printf("Key FC_JOYPAD_BTN_DOWN is ");
      break;
    case FC_JOYPAD_100ASK_BTN_LEFT:
      printf("Key FC_JOYPFC_JOYPAD_BTN_LEFT is ");
      break;
    case FC_JOYPAD_100ASK_BTN_RIGHT:
      printf("Key FC_JOYPAD_BTN_RIGHT is ");
      break;
    default:
      printf("FC Joypad data error!");
      break;
  }

  if (btn_state == FC_JOYPAD_100ASK_PRESSED)
    printf("pressed!\n");
  else
    printf("release!\n");

}

FINSH_FUNCTION_EXPORT_ALIAS(fc_joypad_100ask, __cmd_100ask_test_fc_joypad, Code to fc joypad test);


