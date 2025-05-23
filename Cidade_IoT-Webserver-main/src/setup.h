#ifndef SETUP_H
#define SETUP_H

#include "hardware/pio.h"
#include "lib/ssd1306.h"
#include "lib/ws2812.h"

// Definição dos pinos dos LEDs
#define LED_PIN CYW43_WL_GPIO_LED_PIN   // GPIO do CI CYW43
#define LED_RED_PIN 13
#define LED_BLUE_PIN 12      
#define LED_GREEN_PIN 11

#define BUTTON_B 6
#define DEBOUNCE_TIME 200000        // Tempo para debounce em us (microssegundos)
static uint32_t last_time_B = 0;    // Tempo da última interrupção do botão B

#define BUZZER_PIN 10

#define WS2812_PIN 7
extern PIO pio;
extern uint sm;

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
extern ssd1306_t ssd;
extern bool cor;

// Pinos do joystick (ADC)
#define ADC_JOYSTICK_X 26  // Pino GP26 (ADC0)
#define ADC_JOYSTICK_Y 27  // Pino GP27 (ADC1)

// Variáveis de sensor
extern int temperatura;
extern int umidade;
extern int oxigenio;

// Estado do buzzer
extern bool buzzer_ligado;

#endif