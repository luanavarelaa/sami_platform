#ifndef DRV_WIFI_H
#define DRV_WIFI_H

// ================= GLOBAL FUNCTIONS =================
void drv_wifi_init(const char* ssid, const char* password);
bool drv_wifi_is_connected();

#endif // DRV_WIFI_H