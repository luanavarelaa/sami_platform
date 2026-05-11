#include <Arduino.h>
#include <stdbool.h>
#include "app_sami.h"
#include "app_monitor.h"
#include "sami_types.h"

bool estado_anterior = false;

void setup() 
{
  app_sami_init();
}

void loop() 
{
  app_monitor_task();
}

