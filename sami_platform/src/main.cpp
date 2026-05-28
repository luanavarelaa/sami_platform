#include <Arduino.h>
#include <stdbool.h>
#include "app_sami.h"

void setup() 
{
  app_sami_init();
}

void loop() 
{
  app_sami_run();
}

