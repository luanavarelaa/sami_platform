#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#include "drv_wifi.h"

void drv_wifi_init(const char* ssid, const char* password) 
{
    Serial.print("\n[DRV_WIFI]Conectando ao Wi-Fi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("\n[DRV_WIFI]Wi-Fi Conectado com sucesso!");
    Serial.print("\n[DRV_WIFI]Endereço IP recebido: ");
    Serial.println(WiFi.localIP());
}

bool drv_wifi_is_connected() 
{
    return (WiFi.status() == WL_CONNECTED);
}