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
    
    // Inicialização do Compartimento 1
    compartment_1.id = COMPARTMENT_1;
    compartment_1.previous_state = false;
    compartment_1.interval = 3; 
    compartment_1.next_dose_time = current_time + 2; 
    compartment_1.early_alert_sent = false;
    compartment_1.exact_alert_sent = false;
    compartment_1.late_alert_sent = false;
    compartment_1.message_pending = false;
    compartment_1.state_to_send = false;

    // Inicialização do Compartimento 2
    compartment_2.id = COMPARTMENT_2;
    compartment_2.previous_state = false;
    compartment_2.interval = 3; 
    compartment_2.next_dose_time = current_time + 2; 
    compartment_2.early_alert_sent = false;
    compartment_2.exact_alert_sent = false;
    compartment_2.late_alert_sent = false;
    compartment_2.message_pending = false;
    compartment_2.state_to_send = false;

    // Inicialização do Compartimento 3
    compartment_3.id = COMPARTMENT_3;
    compartment_3.previous_state = false;
    compartment_3.interval = 3; 
    compartment_3.next_dose_time = current_time + 2; 
    compartment_3.early_alert_sent = false;
    compartment_3.exact_alert_sent = false;
    compartment_3.late_alert_sent = false;
    compartment_3.message_pending = false;
    compartment_3.state_to_send = false;

    // Inicialização do Compartimento 4
    compartment_4.id = COMPARTMENT_4;
    compartment_4.previous_state = false;
    compartment_4.interval = 3; 
    compartment_4.next_dose_time = current_time + 2; 
    compartment_4.early_alert_sent = false;
    compartment_4.exact_alert_sent = false;
    compartment_4.late_alert_sent = false;
    compartment_4.message_pending = false;
    compartment_4.state_to_send = false;
    
    Serial.println("[APP_MONITOR] All 4 compartments initialized successfully.");
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
            Serial.printf("[APP_MONITOR] Compartment %d OPEN at minute: %d\n", (int)compartment + 1, current_time);
            app_monitor_evaluate_schedule(compartment, current_time);
            
            box->next_dose_time = current_time + box->interval;
            
            
            box->early_alert_sent = false;
            box->exact_alert_sent = false;
            box->late_alert_sent = false;
            
            Serial.printf("[APP_MONITOR] Compartment %d next dose rescheduled to: %d\n", (int)compartment + 1, box->next_dose_time);
            break;

        case 0: // CLOSED
            Serial.printf("[APP_MONITOR] Compartment %d CLOSED\n", (int)compartment + 1);
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
}