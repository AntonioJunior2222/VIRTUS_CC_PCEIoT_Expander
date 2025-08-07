# Controle de LEDs com Expansor de I/O SX1509B e Raspberry Pi Pico W

## Autores

- [Antonio Roberto](https://github.com/antoniojunior2222)
- [Ismael Marinho](https://github.com/smalljooj)

## Introdução

Este documento apresenta as funcionalidades desenvolvidas no projeto de controle de periféricos, utilizando um microcontrolador Raspberry Pi Pico W e o expansor de I/O SX1509B, integrados na placa PCEIoT desenvolvida pelo VIRTUS-CC. Realizado no contexto de projetos de Sistemas Embarcados em Campina Grande, o sistema foi concebido para demonstrar uma arquitetura de software modular no controle de dispositivos de entrada e saída.

O principal objetivo do sistema é capturar comandos do usuário de forma precisa por meio de três botões push-button. Estes componentes de entrada, configurados com resistores de pull-down externos, enviam sinais de nível alto ao serem pressionados, permitindo que a lógica de aplicação detecte eventos de clique discretos para acionar as funcionalidades de controle.

Em resposta, o sistema fornece feedback visual através de três LEDs RGB independentes, cujo controle de cor é gerenciado diretamente pelo expansor SX1509B, aliviando o microcontrolador principal dessa tarefa. Por meio dessa estrutura modular, o projeto exemplifica a separação entre o driver de dispositivo (`expander_lib`) e a lógica de aplicação (`integrations`), uma prática fundamental no desenvolvimento de software embarcado robusto e escalável.

## Componentes de Hardware

### 1\. Raspberry Pi Pico W

O microcontrolador principal do projeto, responsável por rodar a lógica principal, se comunicar com o expansor via I²C e gerenciar o fluxo do programa.

### 2\. Expansor de I/O SX1509B

[cite\_start]O SX1509B é um expansor de GPIO (General Purpose Input/Output) de 16 canais que se comunica através de uma interface I²C padrão de 400kHz[cite: 9]. Ele é o componente central para a expansão das portas do Pico.

**Características Técnicas Notáveis:**

  * [cite\_start]**16 I/Os programáveis:** Cada pino pode ser configurado individualmente como entrada ou saída, com pull-ups, pull-downs e polaridade programáveis[cite: 17, 27].
  * [cite\_start]**Driver de LED Integrado:** Possui um motor de hardware para controle avançado de LEDs, permitindo efeitos de intensidade (PWM de 256 passos), piscar (blink) e respirar (fade in/out) sem a necessidade de intervenção do microcontrolador[cite: 13, 23].
  * [cite\_start]**Motor de Varredura de Teclado:** Suporta a leitura de teclados matriciais de até 8x8 (64 teclas)[cite: 14, 26].
  * [cite\_start]**Saída de Interrupção (NINT):** Pode ser configurado para alertar o microcontrolador sobre eventos (como um botão pressionado) através de um pino de interrupção dedicado, otimizando o processamento[cite: 16, 30].

### 3\. LEDs RGB

  * **Quantidade:** 3 unidades.
  * **Tipo:** LEDs RGB de catodo comum. Nesta configuração, os pinos de cor (Vermelho, Verde, Azul) são conectados aos pinos de I/O do SX1509B, e o pino comum é conectado ao GND. O LED acende quando o pino de I/O é levado a nível baixo (0V).

### 4\. Botões (Push-button)

  * **Quantidade:** 3 unidades.
  * **Configuração:** Botões de acionamento momentâneo conectados com **resistores de pull-down** externos. Nesta configuração, o estado normal do pino de entrada é nível baixo (0), e quando o botão é pressionado, o pino é conectado a VCC, resultando em um nível lógico alto (1).

## Conexões Físicas (Wiring)

A comunicação entre o Pico e o SX1509B é feita via I²C. Os periféricos são conectados diretamente aos pinos de I/O do SX1509B.

| Pino do Pico W | Conexão       | Pino do SX1509B |
| :------------- | :------------ | :-------------- |
| `GP4` (SDA)    | Barramento I²C | `SDA`           |
| `GP5` (SCL)    | Barramento I²C | `SCL`           |
| `3V3`          | Alimentação   | `VDDM`          |
| `GND`          | Terra         | `GND`           |

| Periférico      | Conexão                    | Pinos de I/O do SX1509B |
| :-------------- | :------------------------- | :---------------------- |
| **Botão 0** | Entrada com pull-down      | `IO0`                   |
| **Botão 1** | Entrada com pull-down      | `IO1`                   |
| **Botão 2** | Entrada com pull-down      | `IO2`                   |
| **LED 1 (RGB)** | Saída para R, G, B         | `IO5`, `IO6`, `IO7`     |
| **LED 2 (RGB)** | Saída para R, G, B         | `IO8`, `IO9`, `IO10`    |
| **LED 3 (RGB)** | Saída para R, G, B         | `IO13`, `IO14`, `IO15`  |

## Estrutura do Software

O código foi arquitetado de forma modular para separar as responsabilidades, facilitando a manutenção e a reutilização.

### `expander_lib.c` / `expander_lib.h`

Esta é a camada de abstração de hardware (driver) para o SX1509B. Ela contém as funções de baixo nível que manipulam os registradores do chip.

  * `void config_buttons()`: Configura os pinos IO0, IO1 e IO2 como entradas digitais.
  * `void config_leds()`: Configura os pinos IO5-7, IO8-10 e IO13-15 como saídas digitais.
  * `uint8_t get_buttons_state()`: Lê o registrador de dados do SX1509B e retorna um byte onde os 3 bits menos significativos representam o estado atual dos botões (1 para pressionado, 0 para solto).
  * `void sx1509b_set_led(uint8_t led_id, bool r, bool g, bool b)`: Controla um LED RGB completo, definindo o estado (ligado/desligado) para cada um de seus canais de cor.

### `integrations.c` / `integrations.h`

Esta é a camada de lógica da aplicação. Ela utiliza as funções da `expander_lib` para criar o comportamento desejado do sistema.

  * `void init_integrations()`: Função de inicialização que garante que todos os LEDs comecem no estado "desligado".
  * `void handle_button_integrations()`: O coração da lógica interativa. Esta função:
    1.  Lê o estado atual dos botões.
    2.  Compara com o estado anterior para detectar a "borda de subida" (o momento exato em que o botão é pressionado).
    3.  Para cada botão pressionado, atualiza uma máquina de estados de cor para o LED correspondente (ciclando entre Desligado, Vermelho, Verde, Azul e Branco).
    4.  Chama `sx1509b_set_led()` para aplicar a nova cor.
    5.  Imprime uma mensagem no terminal informando qual botão foi pressionado e qual LED foi alterado.

### `main.c`

O ponto de entrada do programa. Sua responsabilidade é mínima e se resume a:

1.  Inicializar o hardware do Pico (I²C, stdio).
2.  Chamar as funções de configuração da biblioteca (`config_buttons`, `config_leds`).
3.  Chamar a função de inicialização da lógica (`init_integrations`).
4.  Entrar em um loop infinito que chama repetidamente `handle_button_integrations()` para processar os eventos.

### Arquivo de Testes

Este projeto inclui um arquivo de testes (não detalhado neste README) cujo objetivo é validar as funções da biblioteca `expander_lib` de forma isolada. Testes unitários são uma prática recomendada para garantir que cada peça do driver (como escrita e leitura de registradores) funcione como esperado antes de ser integrada na aplicação principal.

## Como Compilar e Executar

Este projeto utiliza o SDK do Raspberry Pi Pico e o sistema de compilação CMake.

1.  **Clone o repositório do Pico SDK:**

    ```bash
    git clone https://github.com/raspberrypi/pico-sdk.git
    ```

2.  **Configure a variável de ambiente `PICO_SDK_PATH`** para apontar para o diretório do SDK clonado.

3.  **Clone este projeto:**

    ```bash
    git clone [URL_DO_SEU_REPOSITORIO]
    ```

4.  **Crie a pasta de compilação e compile o projeto:**

    ```bash
    cd [NOME_DA_PASTA_DO_PROJETO]
    mkdir build
    cd build
    cmake ..
    make
    ```

5.  **Transfira o arquivo `.uf2`** gerado na pasta `build` para o seu Pico W (arraste e solte enquanto o Pico está em modo BOOTSEL).

## Funcionalidades Implementadas

  * Ao pressionar o **Botão 0**, a cor do **LED 1** cicla na seguinte ordem: Desligado -\> Vermelho -\> Verde -\> Azul -\> Branco.
  * Ao pressionar o **Botão 1**, a cor do **LED 2** realiza o mesmo ciclo.
  * Ao pressionar o **Botão 2**, a cor do **LED 3** realiza o mesmo ciclo.
  * O terminal serial exibe uma mensagem indicando qual botão foi pressionado a cada evento.
