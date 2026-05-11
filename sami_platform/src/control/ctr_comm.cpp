#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <stdbool.h>
#include <stdint.h>

#include "ctr_comm.h"
#include "drv_wifi.h" 
#include "sami_types.h"

const char* wifi_ssid = "S23deLuana";
const char* wifi_pass = "docedeleite";
const String token = "BBUS-vf6GrNx5DO6JkmhJdkz3Ub8IQsaDOx"; 

void ctr_comm_init(void) 
{
    Serial.println("[COMM] Inicializando comunicações...");

    drv_wifi_init(wifi_ssid, wifi_pass);
}

bool ctr_comm_send_data(box_compartment_t compartment, bool compartment_open) 
{
    if (!drv_wifi_is_connected()) {
        return false; 
    }

    String variable_name;
    
    switch (compartment) {

        case COMPARTMENT_1: 
            variable_name = "compartment_1";
            break;

        case COMPARTMENT_2:
            variable_name = "compartment_2";
            break;

        case COMPARTMENT_3:
            variable_name = "compartment_3";
            break;

        case COMPARTMENT_4:
            variable_name = "compartment_4";
            break;

        default:
            Serial.println("[COMM] Error: Unknown compartment.");
            return false;
    }

    String payload;
    if (compartment_open) {
        payload = "{\"" + variable_name + "\": 1}"; 
    } else {
        payload = "{\"" + variable_name + "\": 0}";
    }

    WiFiClient client;
    HTTPClient http;
    String url = "http://industrial.api.ubidots.com/api/v1.6/devices/sami";
    
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Auth-Token", token);
    
    int httpCode = http.POST(payload); 
    http.end();
    
    if (httpCode == 200 || httpCode == 201) {
        Serial.println("[COMM] Success on Ubidots!");
        return true;
    } else {
        Serial.printf("[COMM] Ubidots Error: %d\n", httpCode);
        return false;
    }
}