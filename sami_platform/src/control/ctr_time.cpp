#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "ctr_time.h"

void ctr_time_init() {
    configTime(-10800, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("[TIME] Sincronizando hora...");
}

int ctr_time_get_total_minutes() {
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    return (p_tm->tm_hour * 60) + p_tm->tm_min;
}