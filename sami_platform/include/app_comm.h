#ifndef APP_COMM_H
#define APP_COMM_H

#include "sami_types.h"

typedef enum {
    EVENT_BOX_OPEN,
    EVENT_DOSE_TIME,
    EVENT_ALERT
} event_type_t;

// ================= GLOBAL FUNCTIONS =================
void app_comm_init(void);
void app_comm_task(bool internet_ok);
void app_comm_enqueue_event(box_compartment_t compartment, event_type_t type, float value);

#endif // APP_COMM_H