#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

#include "app_sami.h"
#include "app_monitor.h"
#include "ctr_button.h"
#include "ctr_led.h"
#include "ctr_comm.h"
#include "sami_types.h"
#include "ctr_time.h"

void app_sami_init (void)
{
    Serial.begin(115200);

    ctr_button_init();
    ctr_led_init();

    if (ctr_comm_init() && ctr_time_init())
    {
        ctr_set_led(GPIO_YELLOW_LED);
    }

    app_monitor_init();

    Serial.println("");
    Serial.println("\nPlaca inicializada com sucesso!");

}

void app_sami_run(void)
{
    void app_monitor_task(void);
}