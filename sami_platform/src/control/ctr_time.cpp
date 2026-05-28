#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "ctr_time.h"

bool ctr_time_init() 
{
    // Sincroniza com o servidor NTP (Brasília: -3h)
    configTime(-10800, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("\n[CTR_TIME] Sincronizando hora...");

    time_t now = time(nullptr);
    int tentativas = 0;
    
    while (now < 1704067200 && tentativas < 20) 
    { 
        delay(500);
        Serial.print(".");
        now = time(nullptr);
        tentativas++;
    }
    
    if (now < 1704067200) {
        Serial.println("\n[CTR_TIME] Falha ao sincronizar a hora.");
        return false;
    }
    
    Serial.println("\n[CTR_TIME] Hora sincronizada com sucesso!");
    return true;
}

int ctr_time_get_total_minutes() 
{
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    return (p_tm->tm_hour * 60) + p_tm->tm_min;
}