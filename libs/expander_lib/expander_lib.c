#include "expander_lib.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdio.h>

// --- DEFINIÇÕES PRIVADAS (SÓ PARA ESTE ARQUIVO) ---
#define I2C_PORT i2c0
#define SX1509B_ADDR 0x3E

// --- FUNÇÕES AUXILIARES (PRIVADAS) ---
// O 'static' limita o escopo destas funções apenas a este arquivo.
static void sx1509b_write_register(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    i2c_write_blocking(I2C_PORT, SX1509B_ADDR, data, 2, false);
}

static uint8_t sx1509b_read_register(uint8_t reg) {
    uint8_t value;
    i2c_write_blocking(I2C_PORT, SX1509B_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, SX1509B_ADDR, &value, 1, false);
    return value;
}

static void sx1509b_set_color(uint8_t pin, bool on) {
    // RegDataA (0x11) para IO0-7, RegDataB (0x10) para IO8-15
    uint8_t reg = (pin < 8) ? 0x11 : 0x10;
    uint8_t bit = pin % 8;
    uint8_t value = sx1509b_read_register(reg);

    if (on) {
        value &= ~(1 << bit); // Nível baixo para acender
    } else {
        value |= (1 << bit);  // Nível alto para apagar
    }
    sx1509b_write_register(reg, value);
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES PÚBLICAS (DECLARADAS NO .h) ---

void config_buttons() {
    uint8_t direction_a = sx1509b_read_register(0x0F);
    direction_a |= (1 << 0) | (1 << 1) | (1 << 2);
    sx1509b_write_register(0x0F, direction_a);
}

void config_leds() {
    uint8_t direction_a = sx1509b_read_register(0x0F);
    uint8_t direction_b = sx1509b_read_register(0x0E);

    direction_a &= ~((1 << 5) | (1 << 6) | (1 << 7)); // LED1
    direction_b &= ~((1 << 0) | (1 << 1) | (1 << 2) | // LED2
                     (1 << 5) | (1 << 6) | (1 << 7)); // LED3

    sx1509b_write_register(0x0F, direction_a);
    sx1509b_write_register(0x0E, direction_b);
}

void sx1509b_set_led(uint8_t led_id, bool r, bool g, bool b) {
    if (led_id == LED1) {
        sx1509b_set_color(5, r);
        sx1509b_set_color(6, g);
        sx1509b_set_color(7, b);
    } else if (led_id == LED2) {
        sx1509b_set_color(8, r);
        sx1509b_set_color(9, g);
        sx1509b_set_color(10, b);
    } else if (led_id == LED3) {
        sx1509b_set_color(13, r);
        sx1509b_set_color(14, g);
        sx1509b_set_color(15, b);
    }
}

// Função foi modificada para retornar o estado dos botões
uint8_t get_buttons_state() {
    uint8_t raw_state = sx1509b_read_register(0x11); // Lê o RegDataA
    uint8_t button_states = 0;

    // Lógica para pull-down (ativo em alto)
    bool botao0 = (raw_state & (1 << 0));
    bool botao1 = (raw_state & (1 << 1));
    bool botao2 = (raw_state & (1 << 2));

    // Monta o byte de retorno
    if (botao0) button_states |= (1 << 0);
    if (botao1) button_states |= (1 << 1);
    if (botao2) button_states |= (1 << 2);
    
    // Opcional: se ainda quiser imprimir para debug
    // printf("B0: %d | B1: %d | B2: %d\r\n", botao0, botao1, botao2);

    return button_states;
}