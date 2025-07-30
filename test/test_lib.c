// integrations.c

#include "inc/test_lib.h"
#include "expander_lib.h" // Inclui a biblioteca do expander
#include <stdio.h>

// --- ESTADO INTERNO DO MÓDULO ---
// 'static' garante que estas variáveis mantenham seus valores entre as chamadas da função
static color_state_t led1_color_state;
static color_state_t led2_color_state;
static color_state_t led3_color_state;
static uint8_t previous_buttons_state = 0;


// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

void init_integrations() {
    printf("Inicializando integracoes... LEDs desligados.\n");
    // Garante que todos os LEDs comecem desligados
    sx1509b_set_led(LED1, false, false, false);
    sx1509b_set_led(LED2, false, false, false);
    sx1509b_set_led(LED3, false, false, false);

    // Inicializa o estado das cores
    led1_color_state = OFF;
    led2_color_state = OFF;
    led3_color_state = OFF;
}

void handle_button_integrations() {
    // 1. Lê o estado atual dos botões da biblioteca
    uint8_t current_buttons = get_buttons_state();

    // --- LÓGICA PARA BOTÃO 0 -> LED 1 ---
    if ((current_buttons & (1 << 0)) && !(previous_buttons_state & (1 << 0))) {
        // MENSAGEM ADICIONADA
        printf("Botao 0 pressionado -> LED 1 mudou de cor.\r\n");

        led1_color_state = (led1_color_state + 1) % 5; // Cicla entre 5 estados
        switch (led1_color_state) {
            case OFF:   sx1509b_set_led(LED1, false, false, false); break;
            case RED:   sx1509b_set_led(LED1, true,  false, false); break;
            case GREEN: sx1509b_set_led(LED1, false, true,  false); break;
            case BLUE:  sx1509b_set_led(LED1, false, false, true ); break;
            case WHITE: sx1509b_set_led(LED1, true,  true,  true ); break;
        }
    }

    // --- LÓGICA PARA BOTÃO 1 -> LED 2 ---
    if ((current_buttons & (1 << 1)) && !(previous_buttons_state & (1 << 1))) {
        // MENSAGEM ADICIONADA
        printf("Botao 1 pressionado -> LED 2 mudou de cor.\r\n");

        led2_color_state = (led2_color_state + 1) % 5; // Cicla entre 5 estados
        switch (led2_color_state) {
            case OFF:   sx1509b_set_led(LED2, false, false, false); break;
            case RED:   sx1509b_set_led(LED2, true,  false, false); break;
            case GREEN: sx1509b_set_led(LED2, false, true,  false); break;
            case BLUE:  sx1509b_set_led(LED2, false, false, true ); break;
            case WHITE: sx1509b_set_led(LED2, true,  true,  true ); break;
        }
    }

    // --- LÓGICA PARA BOTÃO 2 -> LED 3 ---
    if ((current_buttons & (1 << 2)) && !(previous_buttons_state & (1 << 2))) {
        // MENSAGEM ADICIONADA
        printf("Botao 2 pressionado -> LED 3 mudou de cor.\r\n");
        
        led3_color_state = (led3_color_state + 1) % 5; // Cicla entre 5 estados
        switch (led3_color_state) {
            case OFF:   sx1509b_set_led(LED3, false, false, false); break;
            case RED:   sx1509b_set_led(LED3, true,  false, false); break;
            case GREEN: sx1509b_set_led(LED3, false, true,  false); break;
            case BLUE:  sx1509b_set_led(LED3, false, false, true ); break;
            case WHITE: sx1509b_set_led(LED3, true,  true,  true ); break;
        }
    }

    // 2. Guarda o estado atual para a próxima verificação
    previous_buttons_state = current_buttons;
}