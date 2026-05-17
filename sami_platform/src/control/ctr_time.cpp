#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "ctr_time.h"

void ctr_time_init() 
{
    // Sincroniza com o servidor NTP (Brasília: -3h)
    configTime(-10800, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("\n[CTR_TIME] Sincronizando hora...");

    time_t now = time(nullptr);
    while (now < 1704067200) 
    { 
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    
    Serial.println("\n[CTR_TIME] Hora sincronizada com sucesso!");
}

int ctr_time_get_total_minutes() 
{
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    return (p_tm->tm_hour * 60) + p_tm->tm_min;
}