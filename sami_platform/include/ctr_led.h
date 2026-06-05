#ifndef CTR_LED_H  
#define CTR_LED_H  

#include "sami_types.h"
#include "drv_gpio.h"

// ================= GLOBAL FUNCTIONS =================
void ctr_led_init(void);
void ctr_led_set_green(box_compartment_t compartment);
void ctr_led_set_red(box_compartment_t compartment);
void ctr_clear_r_g_led(box_compartment_t compartment);
void ctr_set_led(gpio_name_t compartment);
void ctr_clear_led(gpio_name_t compartment);

#endif /* CTR_LED_H */