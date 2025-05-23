#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"     // Biblioteca para arquitetura Wi-Fi da Pico com CYW43
#include "pico/bootrom.h"

#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lwip/pbuf.h"           // Lightweight IP stack - manipulação de buffers de pacotes de rede
#include "lwip/tcp.h"            // Lightweight IP stack - fornece funções e estruturas para trabalhar com o protocolo TCP
#include "lwip/netif.h"          // Lightweight IP stack - fornece funções e estruturas para trabalhar com interfaces de rede (netif)

#include "setup.h"
#include "src/webserver.h"
#include "lib/ws2812.h"
#include "ws2812.pio.h"
#include "webserver.h"

static uint buzzer_slice;
static bool buzzer_pwm_initialized = false;

// Credenciais WIFI - Troque pelas suas credenciais
#define WIFI_SSID "Lucas"
#define WIFI_PASSWORD "outubro1999"

// ----------------------------- Escopo de funções ------------------------------

void buttons_irq(uint gpio, uint32_t events);
void update_display();
void setup_display();
void setup_matrix();
void gpio_led_bitdog(void);
void setup_button(uint pin);
// -------------------------------------------------------------------------------


int main() {
    // Configurações iniciais
    stdio_init_all();
    srand(time(NULL));
    gpio_led_bitdog();
    setup_button(BUTTON_B);
    setup_display();
    setup_matrix();

    // Inicializa ADC para o joystick
    adc_init();
    adc_gpio_init(ADC_JOYSTICK_X);
    adc_gpio_init(ADC_JOYSTICK_Y);
    adc_set_round_robin(0);
        
    // Inicializa buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    // Inicializa Wi-Fi
    if (cyw43_arch_init()) {
        printf("Falha ao inicializar Wi-Fi\n");
        return -1;
    }
    sleep_ms(1000);  // Espera para estabilização

    cyw43_arch_gpio_put(LED_PIN, 0);
    cyw43_arch_enable_sta_mode();

    // Conexão ao Wi-Fi
    printf("Conectando ao Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 20000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        cyw43_arch_deinit();
        return -1;
    }
    printf("Conectado ao Wi-Fi\n");

    if (netif_default) {
        printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    }

    webserver_init();

    // Loop principal
    while (true) {
        cyw43_arch_poll();
        
        // Leitura do joystick
        adc_select_input(0);
        uint16_t x_raw = adc_read();
        temperatura = (x_raw * 50) / 4095;

        adc_select_input(1);
        uint16_t y_raw = adc_read();
        umidade = (y_raw * 100) / 4095;

        // Controle do buzzer
        // Substitua APENAS esta parte no loop while(true):
        if (buzzer_ligado) {
            if (!buzzer_pwm_initialized) {
                // Configura PWM apenas uma vez
                gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
                buzzer_slice = pwm_gpio_to_slice_num(BUZZER_PIN);
                pwm_config config = pwm_get_default_config();
                pwm_config_set_wrap(&config, 15000);  // Frequência ~1kHz
                pwm_init(buzzer_slice, &config, true);
                pwm_set_gpio_level(BUZZER_PIN, 110);  // 50% duty cycle
                buzzer_pwm_initialized = true;
            }
        } else {
            if (buzzer_pwm_initialized) {
                pwm_set_gpio_level(BUZZER_PIN, 0);  // Desliga o som
            } else {
                gpio_put(BUZZER_PIN, 0);  // Garante desligado
            }
        }

        update_display();
        sleep_ms(100);
    }

    // Esta parte nunca será alcançada devido ao loop infinito,
    // mas é boa prática manter para estruturação do código
    cyw43_arch_deinit();
    return 0;
}

// ---------------------------------- Funções ------------------------------------

void buzzer_beep() {
    for(int i = 0; i < 50; i++) {
        gpio_put(BUZZER_PIN, 1);
        sleep_us(500);
        gpio_put(BUZZER_PIN, 0);
        sleep_us(500);
    }
}

void buttons_irq(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (gpio == BUTTON_B) {
        if (current_time - last_time_B > DEBOUNCE_TIME) {
            reset_usb_boot(0, 0);
            last_time_B = current_time;
            return;
        }
    }
}

void update_display() {
    ssd1306_fill(&ssd, !cor);
    ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);
    char msg_temp[20];
    sprintf(msg_temp, "Temperatura %d", temperatura);
    ssd1306_draw_string(&ssd, msg_temp, 8, 6);
    char msg_umid[20];
    sprintf(msg_umid, "Umidade %d", umidade);
    ssd1306_draw_string(&ssd, msg_umid, 8, 20);
    
    ssd1306_send_data(&ssd);
    sleep_ms(735);
}

void setup_display() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

void setup_matrix() {
    uint offset = pio_add_program(pio, &pio_matrix_program);
    pio_matrix_program_init(pio, sm, offset, WS2812_PIN);
    clear_matrix(pio, sm);
    sleep_ms(100);
}

// Configura os pinos GPIO para acionamento dos LEDs da BitDogLab
void gpio_led_bitdog(void) {
    gpio_init(LED_BLUE_PIN);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
    gpio_put(LED_BLUE_PIN, 0);
    
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_put(LED_GREEN_PIN, 0);
    
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_put(LED_RED_PIN, 0);
}

// Configura o pino GPIO do botão push-up da BitDogLab
void setup_button(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &buttons_irq);
}