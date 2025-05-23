#include "setup.h"

// Variáveis globais
extern int temperatura;
extern int umidade;
extern bool buzzer_ligado;

// Pinos do joystick (ADC)
#define ADC_JOYSTICK_X 26  // Pino GP26 (ADC0)
#define ADC_JOYSTICK_Y 27  // Pino GP27 (ADC1)
#define BUZZER_PIN 10      // Pino do buzzer
#define BUZZER_PWM_WRAP 10000  // Valor para controle de frequência
extern uint buzzer_slice_num;   

// ... (outras variáveis existentes)

bool buzzer_ligado = false;  // Definição da variável global

PIO pio = pio0;
uint sm = 0;

ssd1306_t ssd;
bool cor = true;

int temperatura = 39;
int umidade = 49;
int oxigenio = 14;