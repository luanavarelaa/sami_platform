#ifndef APP_MONITOR_H
#define APP_MONITOR_H

#include "sami_types.h"

// ================= GLOBAL FUNCTIONS =================
void app_monitor_init(void);
void app_monitor_task(void);
void app_monitor_update_from_cloud(box_compartment_t compartment, bool is_active, int new_interval);

#endif // APP_MONITOR_H