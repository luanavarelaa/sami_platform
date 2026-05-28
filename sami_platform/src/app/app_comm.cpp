// ================= INCLUDES =================
#include <Arduino.h>
#include "app_comm.h"
#include "app_monitor.h"
#include "ctr_comm.h"
#include "ctr_time.h"

// ================= DEFINES =================
#define OFFLINE_BUFFER_SIZE     20
#define CLOUD_SYNC_INTERVAL_MS  5000
#define UBI_RATE_LIMIT_MS       1000 // 1 segundo de intervalo entre envios

// ================= TYPEDEFS & STRUCTS =================
typedef struct 
{
    box_compartment_t compartment;
    event_type_t type;
    float value;
    int timestamp; // Timestamp em minutos 
} offline_event_t;

// ================= LOCAL VARIABLES =================
static offline_event_t event_buffer[OFFLINE_BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;
static int buffer_count = 0;

static unsigned long last_cloud_sync = 0;
static unsigned long last_event_send = 0;

// ================= LOCAL FUNCTION PROTOTYPES =================
static void app_comm_process_buffer_item(void);
static void app_comm_fetch_configs(void);
static bool app_comm_dispatch_to_cloud(offline_event_t event);

// ================= LOCAL FUNCTIONS =================

static bool app_comm_dispatch_to_cloud(offline_event_t event)
{
    bool success = false;
    
    switch (event.type) 
    {
        case EVENT_BOX_OPEN:
            // Converte o float de volta para booleano
            success = ctr_comm_send_data(event.compartment, (event.value > 0.0f));
            break;
            
        case EVENT_DOSE_TIME:
            success = ctr_comm_send_next_dose_time(event.compartment, event.value);
            break;
            
        case EVENT_ALERT:
            // Converte o float para inteiro do código de alerta
            success = ctr_comm_send_alert(event.compartment, (int)event.value);
            break;
    }
    
    return success;
}

static void app_comm_process_buffer_item(void)
{
    if (buffer_count == 0) {
        return;
    }

    offline_event_t ev = event_buffer[buffer_tail];
    
    if (app_comm_dispatch_to_cloud(ev)) 
    {
        buffer_tail = (buffer_tail + 1) % OFFLINE_BUFFER_SIZE;
        buffer_count--;
        Serial.println("[APP_COMM] Queued event sent successfully!");
    }
    else 
    {
        Serial.println("[APP_COMM] Failed to send event. Keeping in buffer.");
    }
}

static void app_comm_fetch_configs(void)
{
    for (int i = 0; i < 4; i++) 
    {
        box_compartment_t comp = (box_compartment_t)i;
        
        float cloud_start = ctr_comm_get_config(comp, "start");
        float cloud_interval = ctr_comm_get_config(comp, "interval");

        if (cloud_start != -1.0 && cloud_interval != -1.0) 
        {
            app_monitor_update_from_cloud(comp, (cloud_start == 1.0), (int)cloud_interval);
        }
    }
}

// ================= GLOBAL FUNCTIONS =================

void app_comm_init(void) 
{
    buffer_head = 0;
    buffer_tail = 0;
    buffer_count = 0;
    last_cloud_sync = 0;
    last_event_send = 0;
    
    Serial.println("[APP_COMM] Communication module initialized.");
}

void app_comm_task(bool internet_ok) 
{
    if (!internet_ok) {
        return; 
    }

    if (buffer_count > 0 && (millis() - last_event_send > UBI_RATE_LIMIT_MS)) 
    {
        app_comm_process_buffer_item();
        last_event_send = millis();
    }

    if (millis() - last_cloud_sync > CLOUD_SYNC_INTERVAL_MS) 
    {
        app_comm_fetch_configs();
        last_cloud_sync = millis();
    }
}

void app_comm_enqueue_event(box_compartment_t compartment, event_type_t type, float value) 
{
    if (buffer_count >= OFFLINE_BUFFER_SIZE) 
    {
        buffer_tail = (buffer_tail + 1) % OFFLINE_BUFFER_SIZE;
        buffer_count--;
        Serial.println("[APP_COMM] WARNING: Buffer full! Overwriting oldest event.");
    }

    event_buffer[buffer_head].compartment = compartment;
    event_buffer[buffer_head].type = type;
    event_buffer[buffer_head].value = value;
    event_buffer[buffer_head].timestamp = ctr_time_get_total_minutes();

    buffer_head = (buffer_head + 1) % OFFLINE_BUFFER_SIZE;
    buffer_count++;
    
    Serial.printf("[APP_COMM] Event %d queued. Total in buffer: %d\n", type, buffer_count);
}