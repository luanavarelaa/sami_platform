#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "app_monitor.h"
#include "ctr_button.h"
#include "ctr_comm.h"
#include "ctr_time.h"
#include "sami_types.h" 
#include "ctr_led.h"

#define ANTECEDENT_TIME     1
#define ANTECEDENT_ALERT    1
#define TIME_ALERT          5
#define EXCESS_TIME         2
#define EXCESS_ALERT        4
#define TOOK_ON_TIME        2
#define NOT_TOOK_ON_TIME    3

typedef struct 
{
    box_compartment_t id;
    bool previous_state;
    int interval;
    int next_dose_time;
    bool early_alert_sent;
    bool exact_alert_sent; 
    bool late_alert_sent;
    bool message_pending;
    bool state_to_send;
} compartment_monitor_t;

static compartment_monitor_t compartment_1;
static compartment_monitor_t compartment_2;
static compartment_monitor_t compartment_3;
static compartment_monitor_t compartment_4;

void app_monitor_init(void) 
{
    ctr_time_init();
    int current_time = ctr_time_get_total_minutes();
    
    compartment_monitor_t* boxes[4] = {&compartment_1, &compartment_2, &compartment_3, &compartment_4};
    box_compartment_t ids[4] = {COMPARTMENT_1, COMPARTMENT_2, COMPARTMENT_3, COMPARTMENT_4};

    for (int i = 0; i < 4; i++) 
    {
        boxes[i]->id = ids[i];
        boxes[i]->previous_state = false;

        float cloud_interval = ctr_comm_get_config(ids[i], "interval");
        if (cloud_interval > 0.0) 
        {
            boxes[i]->interval = (int)(cloud_interval * 60); 
        } 
        else 
        {
            boxes[i]->interval = 3;
        }

        float cloud_start = ctr_comm_get_config(ids[i], "start");
        if (cloud_start >= 0.0) 
        {
            boxes[i]->next_dose_time = (int)(cloud_start * 60); 
        } 
        else 
        {
            boxes[i]->next_dose_time = current_time + 2; 
        }

        boxes[i]->early_alert_sent = false;
        boxes[i]->exact_alert_sent = false;
        boxes[i]->late_alert_sent = false;
        boxes[i]->message_pending = false;
        boxes[i]->state_to_send = false;

        Serial.printf("[APP_MONITOR] Caixa %d -> Intervalo: %d min | Primeira dose no minuto: %d do dia\n", 
                      i + 1, boxes[i]->interval, boxes[i]->next_dose_time);
    }
    
    Serial.println("[APP_MONITOR] Todos os compartimentos configurados via nuvem.");
}

void app_monitor_sync_interval_changes(box_compartment_t compartment)
{
    compartment_monitor_t* box = NULL;
    switch (compartment) 
    {
        case COMPARTMENT_1: box = &compartment_1; break;
        case COMPARTMENT_2: box = &compartment_2; break;
        case COMPARTMENT_3: box = &compartment_3; break;
        case COMPARTMENT_4: box = &compartment_4; break;
        default: return;
    }

    float cloud_interval_hours = ctr_comm_get_config(compartment, "interval");
    
    if (cloud_interval_hours > 0.0)
    {
        int cloud_interval_minutes = (int)(cloud_interval_hours * 60);

        if (cloud_interval_minutes != box->interval)
        {
            Serial.printf("[APP_MONITOR] Novo intervalo detectado para caixa %d: %d min\n", 
                          (int)compartment + 1, cloud_interval_minutes);
            
            int diferenca = cloud_interval_minutes - box->interval;
            box->interval = cloud_interval_minutes;
            box->next_dose_time += diferenca; 
            
            box->early_alert_sent = false;
            box->exact_alert_sent = false;
            box->late_alert_sent = false;
        }
    }
}

void app_monitor_check_alerts(box_compartment_t compartment, int current_time) 
{
    compartment_monitor_t* box = NULL;
    switch (compartment) 
    {
        case COMPARTMENT_1: box = &compartment_1; break;
        case COMPARTMENT_2: box = &compartment_2; break;
        case COMPARTMENT_3: box = &compartment_3; break;
        case COMPARTMENT_4: box = &compartment_4; break;
        default: return;
    }

    if (current_time == (box->next_dose_time - ANTECEDENT_TIME) && !box->early_alert_sent) 
    {
        ctr_comm_send_alert(compartment, ANTECEDENT_ALERT); 
        box->early_alert_sent = true;
    }
    
    if (current_time == box->next_dose_time && !box->exact_alert_sent) 
    {
        ctr_led_set_green(compartment);
        ctr_comm_send_alert(compartment, TIME_ALERT); 
        box->exact_alert_sent = true;
    }
    
    if (current_time >= (box->next_dose_time + EXCESS_TIME) && !box->late_alert_sent) 
    {
        ctr_led_set_red(compartment);
        ctr_comm_send_alert(compartment, EXCESS_ALERT); 
        box->late_alert_sent = true;
    }
}

void app_monitor_evaluate_schedule(box_compartment_t compartment, int current_time) 
{
    compartment_monitor_t* box = NULL;
    switch (compartment) 
    {
        case COMPARTMENT_1: box = &compartment_1; break;
        case COMPARTMENT_2: box = &compartment_2; break;
        case COMPARTMENT_3: box = &compartment_3; break;
        case COMPARTMENT_4: box = &compartment_4; break;
        default: return;
    }

    if (current_time > (box->next_dose_time + EXCESS_TIME)) 
    {
        ctr_comm_send_alert(compartment, NOT_TOOK_ON_TIME);
        ctr_clear_r_g_led(compartment); 
    } 
    else if (current_time >= (box->next_dose_time - ANTECEDENT_TIME)) 
    {
        ctr_comm_send_alert(compartment, TOOK_ON_TIME); 
        ctr_clear_r_g_led(compartment); 
    }
}

void app_monitor_check_box(box_compartment_t compartment) 
{
    int current_time = ctr_time_get_total_minutes();
    app_monitor_check_alerts(compartment, current_time);

    compartment_monitor_t* box = NULL;
    switch (compartment) 
    {
        case COMPARTMENT_1: box = &compartment_1; break;
        case COMPARTMENT_2: box = &compartment_2; break;
        case COMPARTMENT_3: box = &compartment_3; break;
        case COMPARTMENT_4: box = &compartment_4; break;
        default: return;
    }

    bool current_state = ctr_check_compartment_is_open(box->id);
    if (current_state == box->previous_state) {
        return; 
    }

    switch (current_state ? 1 : 0) 
    {
        case 1: // OPEN
            Serial.printf("[APP_MONITOR] Caixa %d ABERTA no minuto: %d\n", (int)compartment + 1, current_time);
            app_monitor_evaluate_schedule(compartment, current_time);
            
            box->next_dose_time = current_time + box->interval;
            
            box->early_alert_sent = false;
            box->exact_alert_sent = false;
            box->late_alert_sent = false;
            
            Serial.printf("[APP_MONITOR] Caixa %d reagendada automaticamente para o minuto: %d\n", (int)compartment + 1, box->next_dose_time);
            break;

        case 0: // CLOSED
            Serial.printf("[APP_MONITOR] Caixa %d FECHADA\n", (int)compartment + 1);
            break;
    }
    
    box->state_to_send = current_state;
    box->message_pending = true;
    box->previous_state = current_state;
}

void app_monitor_send_messages(box_compartment_t compartment) 
{
    compartment_monitor_t* box = NULL;
    switch (compartment) 
    {
        case COMPARTMENT_1: box = &compartment_1; break;
        case COMPARTMENT_2: box = &compartment_2; break;
        case COMPARTMENT_3: box = &compartment_3; break;
        case COMPARTMENT_4: box = &compartment_4; break;
        default: return;
    }

    if (!box->message_pending) {
        return; 
    }

    if (ctr_comm_send_data(box->id, box->state_to_send)) {
        box->message_pending = false; 
    }
}

void app_monitor_task(void) 
{
    app_monitor_check_box(COMPARTMENT_1);
    app_monitor_send_messages(COMPARTMENT_1);
    app_monitor_sync_interval_changes(COMPARTMENT_1);
}