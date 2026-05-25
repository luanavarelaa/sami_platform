#ifndef CTR_COMM_H
#define CTR_COMM_H

#include "sami_types.h"

void ctr_comm_init(void);

bool ctr_comm_send_data(box_compartment_t compartment, bool compartment_open) ;

bool ctr_comm_send_alert(box_compartment_t compartment, int alert_code);

float ctr_comm_get_config(box_compartment_t compartment, String type);

#endif