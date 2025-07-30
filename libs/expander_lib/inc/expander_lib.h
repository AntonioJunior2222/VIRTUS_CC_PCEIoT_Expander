#ifndef SX1509B_H
#define SX1509B_H

#include <stdbool.h>
#include <stdint.h>

// Definições públicas dos LEDs para serem usadas no main.c
// Você pode mudar esses valores se os LEDs estiverem em outros pinos
#define LED1 5
#define LED2 8
#define LED3 13

// --- PROTÓTIPOS DAS FUNÇÕES PÚBLICAS ---

/**
 * @brief Configura os pinos IO0, IO1, IO2 como entradas para botões.
 * Desabilita pull-ups e pull-downs internos.
 */
void config_buttons();

/**
 * @brief Configura os pinos dos LEDs RGB como saídas.
 */
void config_leds();

/**
 * @brief Controla um LED RGB completo.
 * @param led_id O ID do LED (LED1, LED2, ou LED3).
 * @param r Estado do canal vermelho (1 = ligado, 0 = desligado).
 * @param g Estado do canal verde.
 * @param b Estado do canal azul.
 */
void sx1509b_set_led(uint8_t led_id, bool r, bool g, bool b);

/**
 * @brief Lê o estado dos botões.
 * @return Um uint8_t onde cada bit representa um botão (bit 0 = botão 0, etc.).
 */
uint8_t get_buttons_state();

#endif