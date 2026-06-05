// ================= INCLUDES =================
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <stdbool.h>
#include <stdint.h>
#include "ctr_comm.h"
#include "drv_wifi.h" 
#include "drv_gpio.h"
#include "sami_types.h"

// ================= DEFINES =================
#define UBIDOTS_URL "http://industrial.api.ubidots.com/api/v1.6/devices/sami"

// ================= TYPEDEFS & STRUCTS =================

// ================= LOCAL VARIABLES =================
static const char* wifi_ssid = "S23deLuana";
static const char* wifi_pass = "docedeleite";
static const String token = "BBUS-vf6GrNx5DO6JkmhJdkz3Ub8IQsaDOx"; 

// ================= LOCAL FUNCTION PROTOTYPES =================

// ================= LOCAL FUNCTIONS =================

// ================= GLOBAL FUNCTIONS =================
bool ctr_comm_init(void) 
{
    Serial.println("\n[CTR_COMM] Initializing communications...");
    drv_wifi_init(wifi_ssid, wifi_pass);
    return drv_wifi_is_connected();
}

bool ctr_comm_send_data(box_compartment_t compartment, bool compartment_open) 
{
    if (!drv_wifi_is_connected()) return false; 

    String variable_name;
    switch (compartment) 
    {
        case COMPARTMENT_1: variable_name = "compartment_1"; break;
        case COMPARTMENT_2: variable_name = "compartment_2"; break;
        case COMPARTMENT_3: variable_name = "compartment_3"; break;
        case COMPARTMENT_4: variable_name = "compartment_4"; break;
        default:
            Serial.println("\n[CTR_COMM] Error: Unknown compartment.");
            return false;
    }

    String payload = compartment_open ? "{\"" + variable_name + "\": 1}" : "{\"" + variable_name + "\": 0}";

    WiFiClient client;
    HTTPClient http;
    
    http.begin(client, UBIDOTS_URL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Auth-Token", token);
    
    int httpCode = http.POST(payload); 
    http.end();
    
    if (httpCode == 200 || httpCode == 201) 
    {
        Serial.println("\n[CTR_COMM] Success on Ubidots!");
        return true;
    } else {
        Serial.printf("\n[CTR_COMM] Ubidots Error: %d\n", httpCode);
        return false;
    }
}

bool ctr_comm_send_alert(box_compartment_t compartment, int alert_code) 
{
    if (!drv_wifi_is_connected()) return false; 
    
    String variable_name = "alert_status_" + String((int)compartment + 1);
    String payload = "{\"" + variable_name + "\": " + String(alert_code) + "}";

    WiFiClient client;
    HTTPClient http;
    
    http.begin(client, UBIDOTS_URL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Auth-Token", token);
    
    int httpCode = http.POST(payload); 
    http.end();
    
    return (httpCode == 200 || httpCode == 201);
}

float ctr_comm_get_config(box_compartment_t compartment, String type) 
{
    if (!drv_wifi_is_connected()) return -1.0;

    WiFiClient client;
    HTTPClient http;
    
    String variable_name = type + "_" + String((int)compartment + 1);
    String url = String(UBIDOTS_URL) + "/" + variable_name + "/lv";
    
    http.begin(client, url);
    http.addHeader("X-Auth-Token", token);
    
    int httpCode = http.GET();
    float value = -1.0;
    
    if (httpCode == 200 || httpCode == 201) 
    {
        String response = http.getString();
        value = response.toFloat();
    }
    else 
    {
        Serial.printf("\n[CTR_COMM] Failed to read %s from Ubidots. HTTP Error: %d\n", variable_name.c_str(), httpCode);
    }
    
    http.end();
    
    return value;
}

bool ctr_comm_send_next_dose_time(box_compartment_t compartment, float time_value) 
{
    if (!drv_wifi_is_connected()) return false; 

    String variable_name = "time_dose_" + String((int)compartment + 1);
    String payload = "{\"" + variable_name + "\": " + String(time_value) + "}";

    WiFiClient client;
    HTTPClient http;
    
    http.begin(client, UBIDOTS_URL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Auth-Token", token);
    
    int httpCode = http.POST(payload); 
    http.end();
    
    return (httpCode == 200 || httpCode == 201);
}

bool ctr_comm_is_connected(void) 
{
    return drv_wifi_is_connected(); 
}