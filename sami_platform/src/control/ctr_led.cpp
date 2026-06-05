// ================= INCLUDES =================
#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include "ctr_led.h"
#include "drv_gpio.h"
#include "sami_types.h"

// ================= DEFINES =================

// ================= TYPEDEFS & STRUCTS =================

// ================= LOCAL VARIABLES =================

// ================= LOCAL FUNCTION PROTOTYPES =================

// ================= LOCAL FUNCTIONS =================

// ================= GLOBAL FUNCTIONS =================
void ctr_led_init(void)
{
    drv_gpio_out_config(GPIO_BLUE_LED);
    drv_gpio_out_config(GPIO_GREEN_LED_1);
    drv_gpio_out_config(GPIO_RED_LED_1);
    drv_gpio_out_config(GPIO_YELLOW_LED);
}

void ctr_led_set_green(box_compartment_t compartment)
{
    switch (compartment)
    {
        case COMPARTMENT_1:
            drv_gpio_set_pin(GPIO_GREEN_LED_1);
            drv_gpio_clear_pin(GPIO_RED_LED_1);
            break;
        default: break;
    }
}

void ctr_led_set_red(box_compartment_t compartment)
{
    switch (compartment)
    {
        case COMPARTMENT_1:
            drv_gpio_set_pin(GPIO_RED_LED_1);
            drv_gpio_clear_pin(GPIO_GREEN_LED_1);
            break;
        default: break;
    }
}

void ctr_clear_r_g_led(box_compartment_t compartment)
{
    switch (compartment)
    {
        case COMPARTMENT_1:
            drv_gpio_clear_pin(GPIO_RED_LED_1);
            drv_gpio_clear_pin(GPIO_GREEN_LED_1);
            break;
        default: break;
    }
}

void ctr_set_led(gpio_name_t pin_name)
{
    drv_gpio_set_pin(pin_name);
}

void ctr_clear_led(gpio_name_t pin_name)
{
    drv_gpio_clear_pin(pin_name);
}