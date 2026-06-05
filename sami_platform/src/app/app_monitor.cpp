// ================= INCLUDES =================
#include <Arduino.h>
#include "app_monitor.h"
#include "app_comm.h"   
#include "ctr_time.h"
#include "ctr_led.h"
#include "ctr_button.h"
#include "drv_gpio.h"

// ================= DEFINES =================
#define ANTECEDENT_TIME     1
#define ANTECEDENT_ALERT    1
#define TIME_ALERT          5
#define EXCESS_TIME         2
#define EXCESS_ALERT        4
#define TOOK_ON_TIME        2
#define NOT_TOOK_ON_TIME    3

// ================= TYPEDEFS & STRUCTS =================
typedef struct 
{
    box_compartment_t id;
    bool previous_state;
    int interval;
    int next_dose_time;
    bool early_alert_sent;
    bool exact_alert_sent; 
    bool late_alert_sent;
    bool is_active;
} compartment_monitor_t;

// ================= LOCAL VARIABLES =================
static compartment_monitor_t compartment_1;
static compartment_monitor_t compartment_2;
static compartment_monitor_t compartment_3;
static compartment_monitor_t compartment_4;

// ================= LOCAL FUNCTION PROTOTYPES =================
static void app_monitor_check_alerts(box_compartment_t compartment, compartment_monitor_t* box, int current_time);
static void app_monitor_evaluate_schedule(box_compartment_t compartment, compartment_monitor_t* box, int current_time);
static void app_monitor_check_box(box_compartment_t compartment, compartment_monitor_t* box);

// ================= LOCAL FUNCTIONS =================

static void app_monitor_check_alerts(box_compartment_t compartment, compartment_monitor_t* box, int current_time) 
{
    if (!box->is_active) {
        return;
    }

    if (current_time == (box->next_dose_time - ANTECEDENT_TIME) && !box->early_alert_sent) 
    {
        app_comm_enqueue_event(compartment, EVENT_ALERT, ANTECEDENT_ALERT); 
        box->early_alert_sent = true;
    }
    
    if (current_time == box->next_dose_time && !box->exact_alert_sent) 
    {
        ctr_led_set_green(compartment);
        app_comm_enqueue_event(compartment, EVENT_ALERT, TIME_ALERT); 
        box->exact_alert_sent = true;
    }
    
    if (current_time >= (box->next_dose_time + EXCESS_TIME) && !box->late_alert_sent) 
    {
        ctr_led_set_red(compartment);
        app_comm_enqueue_event(compartment, EVENT_ALERT, EXCESS_ALERT); 
        box->late_alert_sent = true;
    }
}

static void app_monitor_evaluate_schedule(box_compartment_t compartment, compartment_monitor_t* box, int current_time) 
{
    if (!box->is_active) {
        return;
    }

    if (current_time > (box->next_dose_time + EXCESS_TIME)) 
    {
        app_comm_enqueue_event(compartment, EVENT_ALERT, NOT_TOOK_ON_TIME);
        ctr_clear_r_g_led(compartment); 
    } 
    else if (current_time >= (box->next_dose_time - ANTECEDENT_TIME)) 
    {
        app_comm_enqueue_event(compartment, EVENT_ALERT, TOOK_ON_TIME); 
        ctr_clear_r_g_led(compartment); 
    }
}

static void app_monitor_check_box(box_compartment_t compartment, compartment_monitor_t* box) 
{
    int current_time = ctr_time_get_total_minutes();
    app_monitor_check_alerts(compartment, box, current_time);

    bool current_state = ctr_check_compartment_is_open(box->id);
    if (current_state == box->previous_state) {
        return; 
    }

    box->previous_state = current_state;

    if (current_state) 
    { 
        // OPEN
        Serial.printf("[APP_MONITOR] Box %d OPENED at minute: %d\n", (int)compartment + 1, current_time);
        
        if (box->is_active) 
        {
            app_monitor_evaluate_schedule(compartment, box, current_time);
            
            box->next_dose_time = current_time + box->interval;
            
            int hours = (box->next_dose_time / 60) % 24; 
            int minutes = box->next_dose_time % 60;
            float decimal_time = hours + (minutes / 100.0);

            app_comm_enqueue_event(compartment, EVENT_DOSE_TIME, decimal_time);
            
            box->early_alert_sent = false;
            box->exact_alert_sent = false;
            box->late_alert_sent = false;
            
            Serial.printf("[APP_MONITOR] Box %d rescheduled to: %.2f\n", (int)compartment + 1, decimal_time);
        }
        
        app_comm_enqueue_event(compartment, EVENT_BOX_OPEN, 1.0);
    } 
    else 
    {
        // CLOSED
        Serial.printf("[APP_MONITOR] Box %d CLOSED\n", (int)compartment + 1);
        app_comm_enqueue_event(compartment, EVENT_BOX_OPEN, 0.0);
    }
}

// ================= GLOBAL FUNCTIONS =================

void app_monitor_init(void) 
{
    compartment_monitor_t* boxes[4] = {&compartment_1, &compartment_2, &compartment_3, &compartment_4};
    box_compartment_t ids[4] = {COMPARTMENT_1, COMPARTMENT_2, COMPARTMENT_3, COMPARTMENT_4};

    for (int i = 0; i < 4; i++) 
    {
        boxes[i]->id = ids[i];
        boxes[i]->previous_state = false;
        boxes[i]->interval = 3; // Fallback
        boxes[i]->next_dose_time = 0;
        boxes[i]->early_alert_sent = false;
        boxes[i]->exact_alert_sent = false;
        boxes[i]->late_alert_sent = false;
        boxes[i]->is_active = false; 
    }
    
    Serial.println("[APP_MONITOR] Monitoring module initialized (Waiting for cloud).");
}

void app_monitor_task(void) 
{
    app_monitor_check_box(COMPARTMENT_1, &compartment_1);
    app_monitor_check_box(COMPARTMENT_2, &compartment_2);
    app_monitor_check_box(COMPARTMENT_3, &compartment_3);
    app_monitor_check_box(COMPARTMENT_4, &compartment_4);
}

void app_monitor_update_from_cloud(box_compartment_t compartment, bool is_active, int new_interval) 
{
    compartment_monitor_t* box = NULL;
    switch(compartment) 
    {
        case COMPARTMENT_1: box = &compartment_1; break;
        case COMPARTMENT_2: box = &compartment_2; break;
        case COMPARTMENT_3: box = &compartment_3; break;
        case COMPARTMENT_4: box = &compartment_4; break;
        default: return;
    }

    if (is_active && !box->is_active) 
    {
        box->is_active = true;
        box->interval = new_interval;
        box->next_dose_time = ctr_time_get_total_minutes(); 
        
        int hours = (box->next_dose_time / 60) % 24; 
        int minutes = box->next_dose_time % 60;
        float decimal_time = hours + (minutes / 100.0);
        
        app_comm_enqueue_event(compartment, EVENT_DOSE_TIME, decimal_time);
        
        box->early_alert_sent = false;
        box->exact_alert_sent = false;
        box->late_alert_sent = false;
        
        Serial.printf("[APP_MONITOR] Box %d ACTIVATED via cloud!\n", (int)compartment + 1);
    } 
    else if (is_active && new_interval != box->interval) 
    {
        int difference = new_interval - box->interval;
        box->interval = new_interval;
        box->next_dose_time += difference;
        
        int hours = (box->next_dose_time / 60) % 24; 
        int minutes = box->next_dose_time % 60;
        float decimal_time = hours + (minutes / 100.0);
        
        app_comm_enqueue_event(compartment, EVENT_DOSE_TIME, decimal_time);
        
        box->early_alert_sent = false;
        box->exact_alert_sent = false;
        box->late_alert_sent = false;
        
        Serial.printf("[APP_MONITOR] Box %d changed interval to %d min\n", (int)compartment + 1, new_interval);
    }
    else if (!is_active && box->is_active)
    {
        box->is_active = false;
        ctr_clear_r_g_led(compartment);
        Serial.printf("[APP_MONITOR] Box %d DEACTIVATED via cloud.\n", (int)compartment + 1);
    }
}