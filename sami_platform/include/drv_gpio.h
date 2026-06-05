#ifndef DRV_GPIO_H  
#define DRV_GPIO_H

#include <stdint.h>

// ================= TYPEDEFS & STRUCTS =================
typedef enum
{
    GPIO_BLUE_LED,
    GPIO_BUTTON_1,
    GPIO_GREEN_LED_1,
    GPIO_RED_LED_1,
    GPIO_YELLOW_LED
} gpio_name_t;

// ================= GLOBAL FUNCTIONS =================
void drv_gpio_out_config(gpio_name_t pin);
void drv_gpio_in_config(gpio_name_t pin);
void drv_gpio_set_pin(gpio_name_t pin);
void drv_gpio_clear_pin(gpio_name_t pin);
uint8_t drv_gpio_get_state_pin(gpio_name_t pin);
void drv_gpio_toggle_pin(gpio_name_t pin);

#endif /* DRV_GPIO_H */