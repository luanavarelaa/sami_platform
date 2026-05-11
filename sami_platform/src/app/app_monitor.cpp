#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "app_monitor.h"
#include "ctr_button.h"
#include "ctr_comm.h"
#include "ctr_time.h"
#include "sami_types.h" 

typedef struct {
    box_compartment_t id;
    bool previous_state;
    int last_opened_at;
} compartment_monitor_t;

static compartment_monitor_t boxes[4];

void app_monitor_init(void) {
    boxes[0] = {COMPARTMENT_1, false, 0};
    boxes[1] = {COMPARTMENT_2, false, 0};
    boxes[2] = {COMPARTMENT_3, false, 0};
    boxes[3] = {COMPARTMENT_4, false, 0};
    
    Serial.println("[APP] Monitoramento de compartimentos iniciado.");
    ctr_time_init();
}

void app_monitor_task(void) {
    for (int i = 0; i < 4; i++) {
        
        bool current_state = ctr_check_compartment_is_open(boxes[i].id);

        if (current_state != boxes[i].previous_state) {
            
            if (current_state) {
                boxes[i].last_opened_at = ctr_time_get_total_minutes();
                Serial.printf("[APP] Compartimento %d ABERTO (Minuto do dia: %d)\n", i + 1, boxes[i].last_opened_at);
            } else {
                Serial.printf("[APP] Compartimento %d FECHADO\n", i + 1);
            }
            ctr_comm_send_data(boxes[i].id, current_state);
    
            boxes[i].previous_state = current_state;
        }
    }
}