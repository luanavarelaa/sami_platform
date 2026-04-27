#include <Arduino.h>

// No ESP-12F (NodeMCU), o LED interno geralmente está no pino D4 (GPIO2)
#define LED_PIN 2 

void setup() {
  // Inicializa a comunicação serial para podermos ver mensagens no monitor
  Serial.begin(115200);
  
  // Configura o pino do LED como saída
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("");
  Serial.println("Placa inicializada com sucesso!");
}

void loop() {
  // Liga o LED (Nota: no ESP8266, o LED interno costuma ligar com LOW)
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED Ligado");
  delay(1000); // Espera 1 segundo
  
  // Desliga o LED
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED Desligado");
  delay(1000); // Espera 1 segundo
}