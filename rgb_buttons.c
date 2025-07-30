#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include "libs/expander_lib/inc/expander_lib.h"

// Definições de hardware da placa
#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

int main() {
    stdio_init_all();

    // Inicialização do barramento I2C na placa
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    sleep_ms(1000);
    printf("Pico W iniciado. Configurando perifericos...\n");

    // Chama as funções de alto nível da nossa biblioteca
    config_buttons();
    config_leds();

    printf("Configuracao concluida. Testando LEDs...\n");

    // Usa as funções da biblioteca para controlar o hardware
    sx1509b_set_led(LED1, true, false, false);  // Vermelho
    sx1509b_set_led(LED2, false, true, false);  // Verde
    sx1509b_set_led(LED3, false, false, true);  // Azul

    while (true) {
        read_buttons();
        sleep_ms(200);
    }
    return 0;
}