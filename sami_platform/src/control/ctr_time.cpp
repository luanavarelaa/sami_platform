// ================= INCLUDES =================
#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "ctr_time.h"

// ================= DEFINES =================
#define TIMESTAMP_JAN_1_2024 1704067200
#define MAX_SYNC_ATTEMPTS 20

// ================= TYPEDEFS & STRUCTS =================
// (Vazio)

// ================= LOCAL VARIABLES =================
// (Vazio)

// ================= LOCAL FUNCTION PROTOTYPES =================
// (Vazio)

// ================= LOCAL FUNCTIONS =================
// (Vazio)

// ================= GLOBAL FUNCTIONS =================
bool ctr_time_init() 
{
    configTime(-10800, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("\n[CTR_TIME] Synchronizing time...");

    time_t now = time(nullptr);
    int attempts = 0;
    
    while (now < TIMESTAMP_JAN_1_2024 && attempts < MAX_SYNC_ATTEMPTS) 
    { 
        delay(500);
        Serial.print(".");
        now = time(nullptr);
        attempts++;
    }
    
    if (now < TIMESTAMP_JAN_1_2024) {
        Serial.println("\n[CTR_TIME] Failed to synchronize time.");
        return false;
    }
    
    Serial.println("\n[CTR_TIME] Time synchronized successfully!");
    return true;
}

int ctr_time_get_total_minutes() 
{
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    return (p_tm->tm_hour * 60) + p_tm->tm_min;
}