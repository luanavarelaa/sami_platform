#ifndef CTR_BUTTON_H  
#define CTR_BUTTON_H

#include <Arduino.h>
#include "sami_types.h"

// ================= GLOBAL FUNCTIONS =================
void ctr_button_init(void);
bool ctr_check_compartment_is_open(box_compartment_t compartment);

#endif /* CTR_BUTTON_H */