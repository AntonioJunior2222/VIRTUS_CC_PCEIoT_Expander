#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include "libs\expander_lib\inc\expander_lib.h"
#include "test\inc\test_lib.h"

// Definições de hardware da placa
#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

int main() {
    // --- SETUP INICIAL ---
    stdio_init_all();
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    sleep_ms(1000);
    
    // --- CONFIGURAÇÃO DOS MÓDULOS ---
    printf("Configurando a biblioteca do expander...\n");
    config_buttons();
    config_leds();
    
    printf("Configurando o estado inicial das integracoes...\n");
    init_integrations(); // Chama a inicialização do nosso novo módulo

    printf("Sistema pronto. Entrando no loop principal.\n");

    // --- LOOP PRINCIPAL ---
    while (true) {
        // Apenas uma função é chamada. Toda a complexidade está encapsulada.
        handle_button_integrations();

        // Pausa para não sobrecarregar o processador
        sleep_ms(20);
    }
    
    return 0;
}