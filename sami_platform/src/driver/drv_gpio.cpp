#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

#include "drv_gpio.h"

#define BLUE_LED_PIN 2

#define BUTTON_1_PIN        D3
#define GREEN_LED_1_PIN     D2
#define RED_LED_1_PIN       D1
#define YELLOW_LED_PIN      D4

void drv_gpio_out_config(gpio_name_t pin)
{
    switch(pin)
    {
        case GPIO_BLUE_LED:
            pinMode(BLUE_LED_PIN, OUTPUT);
            break;

        case GPIO_GREEN_LED_1:
            pinMode(GREEN_LED_1_PIN, OUTPUT);
            break;

        case GPIO_RED_LED_1:
            pinMode(RED_LED_1_PIN, OUTPUT);
            break;

        case GPIO_YELLOW_LED:
            pinMode(YELLOW_LED_PIN, OUTPUT);
            break;

        default:
        	break;
    }
}

void drv_gpio_in_config(gpio_name_t pin)
{
    switch(pin)
    {
        case GPIO_BUTTON_1:
            pinMode(BUTTON_1_PIN, INPUT_PULLUP); 
            break;

        default:
            break;
    }
}

void drv_gpio_set_pin(gpio_name_t pin)
{
    switch(pin)
    {
        case GPIO_BLUE_LED:
            digitalWrite(BLUE_LED_PIN, HIGH);
            break;
            
        case GPIO_GREEN_LED_1:
            digitalWrite(GREEN_LED_1_PIN, HIGH);
            break;

        case GPIO_RED_LED_1:
            digitalWrite(RED_LED_1_PIN, HIGH);
            break;
        
        case GPIO_YELLOW_LED:
            digitalWrite(YELLOW_LED_PIN, HIGH);
            break;

        default:
        	break;

    }
}

void drv_gpio_clear_pin(gpio_name_t pin)
{
    switch(pin)
    {
        case GPIO_BLUE_LED:
            digitalWrite(BLUE_LED_PIN, LOW);
            break;
        
        case GPIO_GREEN_LED_1:
            digitalWrite(GREEN_LED_1_PIN, LOW);
            break;

        case GPIO_RED_LED_1:
            digitalWrite(RED_LED_1_PIN, LOW);
            break;

        case GPIO_YELLOW_LED:
            digitalWrite(YELLOW_LED_PIN, LOW);
            break;

        default:
        	break;

    }
}

uint8_t drv_gpio_get_state_pin(gpio_name_t pin)
{
	uint8_t state = 0;

	switch(pin)
	{
		case GPIO_BLUE_LED:
			state = digitalRead(BLUE_LED_PIN);
			break;

        case GPIO_BUTTON_1:
            state = digitalRead(BUTTON_1_PIN); 
            break;

        case GPIO_GREEN_LED_1:
            state = digitalRead(GREEN_LED_1_PIN);
            break;

        case GPIO_RED_LED_1:
            state = digitalRead(RED_LED_1_PIN);
            break;
        
        case GPIO_YELLOW_LED:
            state = digitalRead(YELLOW_LED_PIN);
            break;

		default:
			break;
	}

	return state;
}

void drv_gpio_toggle_pin(gpio_name_t pin)
{
    switch(pin)
    {
        case GPIO_BLUE_LED:
            digitalWrite(BLUE_LED_PIN, !digitalRead(BLUE_LED_PIN));
            break;

        case GPIO_GREEN_LED_1:
            digitalWrite(GREEN_LED_1_PIN, !digitalRead(GREEN_LED_1_PIN));
            break;

        case GPIO_RED_LED_1:
            digitalWrite(RED_LED_1_PIN, !digitalRead(RED_LED_1_PIN));
            break;

        case GPIO_YELLOW_LED:
            digitalWrite(YELLOW_LED_PIN, !digitalRead(YELLOW_LED_PIN));
            break;

        default:
        	break;
    }
}