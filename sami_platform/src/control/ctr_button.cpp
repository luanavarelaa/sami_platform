// ================= INCLUDES =================
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "ctr_button.h"
#include "drv_gpio.h"
#include "sami_types.h"

// ================= DEFINES =================
#define DEBOUNCE_DELAY_MS 20

// ================= TYPEDEFS & STRUCTS =================
typedef struct 
{
    bool debounced_state;
    bool last_reading;
    unsigned long last_change_time;
} button_state_t;

// ================= LOCAL VARIABLES =================
static button_state_t btn_states[4];

// ================= LOCAL FUNCTION PROTOTYPES =================

// ================= LOCAL FUNCTIONS =================

// ================= GLOBAL FUNCTIONS =================
void ctr_button_init(void)
{
    drv_gpio_in_config(GPIO_BUTTON_1);

    for (int i = 0; i < 4; i++) 
    {
        btn_states[i].debounced_state = false;
        btn_states[i].last_reading = false;
        btn_states[i].last_change_time = 0;
    }
}

bool ctr_check_compartment_is_open(box_compartment_t compartment)
{
    bool current_reading = false;
    int idx = (int)compartment;

    switch (compartment)
    {
        case COMPARTMENT_1:
            current_reading = (drv_gpio_get_state_pin(GPIO_BUTTON_1) == 1); // 1 = OPEN
            break;
        default:
            return false;
    }

    if (current_reading != btn_states[idx].last_reading) 
    {
        btn_states[idx].last_change_time = millis();
    }

    if ((millis() - btn_states[idx].last_change_time) > DEBOUNCE_DELAY_MS) 
    {
        btn_states[idx].debounced_state = current_reading;
    }

    btn_states[idx].last_reading = current_reading;

    return btn_states[idx].debounced_state;
}