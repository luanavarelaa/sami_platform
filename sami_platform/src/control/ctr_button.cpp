#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "ctr_button.h"
#include "drv_gpio.h"
#include "sami_types.h"

void ctr_button_init(void)
{
  drv_gpio_in_config (GPIO_BUTTON_1);
  drv_gpio_in_config (GPIO_BUTTON_2);
}

bool ctr_check_compartment_is_open(box_compartment_t compartment)
{
    switch (compartment)
    {
    case COMPARTMENT_1:
        if (drv_gpio_get_state_pin(GPIO_BUTTON_1) == OPEN)
        {
            drv_gpio_set_pin(GPIO_GREEN_LED_1);
            drv_gpio_clear_pin(GPIO_RED_LED_1);
            return true;
        }
        else
        {
            drv_gpio_set_pin(GPIO_RED_LED_1);
            drv_gpio_clear_pin(GPIO_GREEN_LED_1);
            return false;
        }
        break;
    
    case COMPARTMENT_2:
        if (drv_gpio_get_state_pin(GPIO_BUTTON_2) == OPEN)
        {
            drv_gpio_set_pin(GPIO_GREEN_LED_2);
            drv_gpio_clear_pin(GPIO_RED_LED_2);
            return true;
        }
        else
        {
            drv_gpio_set_pin(GPIO_RED_LED_2);
            drv_gpio_clear_pin(GPIO_GREEN_LED_2);
            return false;
        }
        break;
    
    default:
        break;
    }
    return false;
}

