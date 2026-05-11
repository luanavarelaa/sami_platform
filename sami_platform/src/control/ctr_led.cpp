#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#include "drv_gpio.h"

void ctr_led_init(void)
{
  drv_gpio_out_config( GPIO_BLUE_LED);
  
  drv_gpio_out_config( GPIO_GREEN_LED_1);
  drv_gpio_out_config( GPIO_RED_LED_1);

  drv_gpio_out_config(GPIO_GREEN_LED_2);
  drv_gpio_out_config(GPIO_RED_LED_2);
}