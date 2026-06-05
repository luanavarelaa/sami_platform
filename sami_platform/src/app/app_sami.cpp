// ================= INCLUDES =================
#include <Arduino.h>
#include "app_sami.h"
#include "app_monitor.h"
#include "app_comm.h"
#include "ctr_button.h"
#include "ctr_led.h"
#include "ctr_comm.h"
#include "ctr_time.h"

// ================= DEFINES =================

// ================= TYPEDEFS & STRUCTS =================

// ================= LOCAL VARIABLES =================
static bool previous_internet_state = false;

// ================= LOCAL FUNCTION PROTOTYPES =================

// ================= LOCAL FUNCTIONS =================

// ================= GLOBAL FUNCTIONS =================

void app_sami_init(void)
{
    Serial.begin(115200);

    ctr_button_init();
    ctr_led_init();

    bool internet_ok = ctr_comm_init();
    bool time_ok = false;
    
    if (internet_ok) {
        time_ok = ctr_time_init();
    }

    if (internet_ok && time_ok) {
        ctr_set_led(GPIO_YELLOW_LED); 
        previous_internet_state = true;
    } else {
        ctr_clear_led(GPIO_YELLOW_LED); 
        previous_internet_state = false;
    }

    app_monitor_init();
    app_comm_init();

    Serial.println("\n[APP_SAMI] Sistema S.A.M.I. orquestrado e inicializado com sucesso!");
}

void app_sami_run(void)
{
    bool internet_ok = ctr_comm_is_connected(); 

    if (internet_ok != previous_internet_state) 
    {
        if (internet_ok) 
        {
            Serial.println("[APP_SAMI] Internet restaurada. Sincronizando relógio...");
            
            if (ctr_time_init()) 
            {
                ctr_set_led(GPIO_YELLOW_LED); 
                previous_internet_state = true;
            }
        } 
        else 
        {
            Serial.println("[APP_SAMI] Internet caiu. Apagando LED amarelo.");
            ctr_clear_led(GPIO_YELLOW_LED); 
            previous_internet_state = false;
        }
    }

    app_monitor_task();

    app_comm_task(internet_ok);
}