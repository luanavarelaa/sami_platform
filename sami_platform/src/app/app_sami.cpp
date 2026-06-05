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
// (Vazio)

// ================= TYPEDEFS & STRUCTS =================
// (Vazio)

// ================= LOCAL VARIABLES =================
static bool previous_internet_state = false;

// ================= LOCAL FUNCTION PROTOTYPES =================
// (Vazio)

// ================= LOCAL FUNCTIONS =================
// (Vazio)

// ================= GLOBAL FUNCTIONS =================

void app_sami_init(void)
{
    Serial.begin(115200);

    // 1. Inicializa o hardware (Camada de Controle)
    ctr_button_init();
    ctr_led_init();

    // 2. Tenta inicializar a comunicação e sincronizar o tempo
    bool internet_ok = ctr_comm_init();
    bool time_ok = false;
    
    if (internet_ok) {
        time_ok = ctr_time_init();
    }

    // Se tem internet e o relógio sincronizou, acende o LED
    if (internet_ok && time_ok) {
        ctr_set_led(GPIO_YELLOW_LED); 
        previous_internet_state = true;
    } else {
        // Se a sua função de apagar o led tiver outro nome, basta ajustar aqui
        ctr_clear_led(GPIO_YELLOW_LED); 
        previous_internet_state = false;
    }

    // 3. Inicializa as regras de negócio e mensageiria
    app_monitor_init();
    app_comm_init();

    Serial.println("\n[APP_SAMI] Sistema S.A.M.I. orquestrado e inicializado com sucesso!");
}

void app_sami_run(void)
{
    bool internet_ok = ctr_comm_is_connected(); 

    // Se o status do Wi-Fi mudou (caiu ou voltou)
    if (internet_ok != previous_internet_state) 
    {
        if (internet_ok) 
        {
            Serial.println("[APP_SAMI] Internet restaurada. Sincronizando relógio...");
            
            // Tenta sincronizar o horário novamente
            if (ctr_time_init()) 
            {
                ctr_set_led(GPIO_YELLOW_LED); // Acende o LED ao confirmar a sincronização
                previous_internet_state = true;
            }
        } 
        else 
        {
            Serial.println("[APP_SAMI] Internet caiu. Apagando LED amarelo.");
            ctr_clear_led(GPIO_YELLOW_LED); // Apaga o LED
            previous_internet_state = false;
        }
    }

    // 1. O Monitor SEMPRE roda 
    app_monitor_task();

    // 2. O Mensageiro roda com base na saúde da rede
    app_comm_task(internet_ok);
}