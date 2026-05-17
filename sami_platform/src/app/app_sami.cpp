#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

#include "app_sami.h"
#include "app_monitor.h"
#include "ctr_button.h"
#include "ctr_led.h"
#include "ctr_comm.h"
#include "sami_types.h"

void app_sami_init (void)
{
    Serial.begin(115200);

    ctr_button_init();
    ctr_led_init();

    ctr_comm_init();

    app_monitor_init();

    Serial.println("");
    Serial.println("\nPlaca inicializada com sucesso!");

}