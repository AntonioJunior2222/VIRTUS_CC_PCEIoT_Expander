// integrations.h

#ifndef TEST_LIB_H
#define TEST_LIB_H

// Enum para deixar o controle de cores mais legível
typedef enum {
    OFF,
    RED,
    GREEN,
    BLUE,
    WHITE
} color_state_t;

/**
 * @brief Inicializa o estado dos LEDs, desligando todos.
 * Deve ser chamada uma vez no setup.
 */
void init_integrations();

/**
 * @brief Processa a lógica de interação entre botões e LEDs.
 * Deve ser chamada repetidamente no loop principal.
 */
void handle_button_integrations();


#endif