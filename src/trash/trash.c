#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "interrupt_util.h"
#include "buzzer_util.h"
#include "leds_util.h"
#include "oled_util.h"
#include "adc_util.h"
#include "pwm_util.h"
#include "trash.h"

/*******************************************************************************************************/
Pins this_pins;  // Estrutura para armazenar os pinos utilizados
static char *TRASH_COMPARTMENT_NAMES[] = {"Papel", "Plastico", "Vidro", "Metal", "Organico", "Outros"};  // Nomes dos compartimentos de lixo
static bool RETRY = false;  // Flag para tentar novamente a detecção
static uint8_t LEDS_ON[12] = {3,5,14,15,23,22,21,19,10,9,1,2};  // Lista de LEDs ativos
static int LEDS_ON_COUNT = sizeof(LEDS_ON) / sizeof(LEDS_ON[0]);  // Quantidade de LEDs ativos
static uint8_t COLORS[12][3];  // Matriz para armazenar as cores dos LEDs
static char *DISPLAY_SHOW[] = {"msgmInitial","typeAndcompartment", "detecting", "erroDetection", "seletorState", "", "", "", ""};  // Mensagens do display
static int COMPARTMENT_DETECTED = -1;  // Compartimento detectado (-1 significa nenhum)
static char *STATUS[100] = {"", "", "", "", ""};  // Status do sistema
alarm_id_t ALARM_ID = 0;  // ID do alarme
static bool ERROR_DETECTED = false;  // Flag para erro detectado

/*******************************************************************************************************/
// Funções de controle das luzes do lixo ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void trash_Lights_Add_Colors(int pos, int colorR, int colorG, int colorB){
    COLORS[pos][0] = colorR;  // Define a componente R da cor do LED na posição pos
    COLORS[pos][1] = colorG;  // Define a componente G da cor do LED na posição pos
    COLORS[pos][2] = colorB;  // Define a componente B da cor do LED na posição pos
}
//===========================================================================
static void trash_Lights_Generate_Colors(int colorR, int colorG, int colorB){
    for (int i = 0; i < LEDS_ON_COUNT; i++) {
            trash_Lights_Add_Colors(i, colorR, colorG, colorB);  // Define a mesma cor para todos os LEDs
    }
}
//===========================================================================
static void trash_Update_Lights(){
    ws2812_Map_leds_ON(LEDS_ON, COLORS, LEDS_ON_COUNT, true);  // Atualiza os LEDs com as cores definidas
}
//===========================================================================
static void trash_Lights_Indicating_Compartment(){
    trash_Lights_Generate_Colors(3,0,0);  // Define a cor padrão dos LEDs
    if (COMPARTMENT_DETECTED >= 0){
        trash_Lights_Generate_Colors(200,200,0);  // Define a cor de indicação do compartimento
        int pos_indication = COMPARTMENT_DETECTED+COMPARTMENT_DETECTED;  // Calcula a posição dos LEDs indicadores
        trash_Lights_Add_Colors(pos_indication, 200, 0, 0);  // Define a cor do LED indicador
        trash_Lights_Add_Colors(pos_indication+1, 200, 0, 0);  // Define a cor do LED indicador
    }
    trash_Update_Lights();  // Atualiza os LEDs
}
// Funções de controle do OLED +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void trash_OLED_Display(char type_show[]){
    oled_Clear();  // Limpa o display OLED
    oled_Draw_draw((uint8_t[]){0x18,0xFF,0x42,0x52,0x52,0x5A,0x42,0x3C},(128-11),3,8,8);  // Desenha um ícone no display
    oled_Write_String("----------------", 3, 5);  // Escreve uma linha divisória
    if (strcmp(type_show, DISPLAY_SHOW[0]) == 0) {
        oled_Write_String("JOGUE O SEU", 24, 23);  // Mensagem inicial
        oled_Write_String("LIXO AQUI", 32, 33);  // Mensagem inicial
    }else if (strcmp(type_show, DISPLAY_SHOW[1]) == 0) {
        oled_Write_String("TIPO: ", 3, 22);  // Mostra o tipo de lixo detectado
        oled_Write_String(STATUS[0], 40, 22);  // Mostra o tipo de lixo detectado
        oled_Write_String("COMPART:", 3, 35);  // Mostra o compartimento detectado
        oled_Write_String(STATUS[2], 60, 35);  // Mostra o compartimento detectado
    } else if (strcmp(type_show, DISPLAY_SHOW[2]) == 0) {
        oled_Write_String("DETECTANDO...", 14, 28);  // Mensagem de detecção em andamento
    } else if (strcmp(type_show, DISPLAY_SHOW[3]) == 0) {
        oled_Write_String("ERRO AO DETECTAR", 0, 18);  // Mensagem de erro na detecção
        oled_Write_String("A -> Retentar", 3, 31);  // Instrução para retentar
        oled_Write_String("B -> Cancelar", 3, 44);  // Instrução para cancelar
    }else if (strcmp(type_show, DISPLAY_SHOW[4]) == 0) {
        oled_Write_String("SELETOR:", 36, 22);  // Mensagem de seletor liberado
        oled_Write_String("Liberado", 36, 34);  // Mensagem de seletor liberado
    }
    oled_Write_String("-----------------", 3, 55);  // Escreve uma linha divisória
    oled_Update();  // Atualiza o display
}
// Funções de controle da tampa e câmera +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static bool trash_LID_open_status(){
    int status_LID = read_adc(0);  // Lê o status da tampa
    return status_LID > 3500;  // Retorna true se a tampa estiver aberta
}
//===========================================================================
static char *trash_camera_detect(){
    int object_detected = -1;  // Inicializa o objeto detectado como nenhum
    int cam_Read = rand() % 101;  // Simula a leitura da câmera
    int qtd_tentativs = 200;  // Número máximo de tentativas
    int tentativs = 0;  // Contador de tentativas
    
    trash_OLED_Display(DISPLAY_SHOW[2]);  // Mostra mensagem de detecção
    while(cam_Read != 77){  // Simula a detecção de um objeto
        cam_Read = rand() % 101;
        object_detected = rand() % 6;
        if(tentativs >= qtd_tentativs)break;  // Sai do loop se exceder o número de tentativas
        tentativs++;
    }
    gpio_put(this_pins.PIN_CAMERA, 0);  // Desliga a câmera
    gpio_put(this_pins.PIN_LEDSRGB[2], 0);  // Desliga o LED RGB

    if(tentativs >= qtd_tentativs){
        object_detected = -1;  // Nenhum objeto detectado
        gpio_put(this_pins.PIN_LEDSRGB[0], 1);  // Liga o LED vermelho
        COMPARTMENT_DETECTED = -1;  // Nenhum compartimento detectado
        ERROR_DETECTED = true;  // Flag de erro ativada
        buzzer_play_note(this_pins.PIN_BUZZER,1,300);  // Toca o buzzer
        return "Nao Detectad";
    }
    COMPARTMENT_DETECTED = object_detected;  // Define o compartimento detectado
    gpio_put(this_pins.PIN_LEDSRGB[1], 1);  // Liga o LED verde
    return TRASH_COMPARTMENT_NAMES[object_detected];  // Retorna o nome do compartimento
}
//===========================================================================
static char *trash_disable_camera(){
    return "desativada";  // Desativa a câmera
}
// Funções de controle do motor e compartimentos ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static char *trash_motor_center_rotate(){
    static int duty_cycle[6] = {500, 880, 1260, 1640, 2020, 2400};  // Ciclos de trabalho para o motor
    update_duty_cycle_pwm(this_pins.PIN_MOTOR_CENTER, duty_cycle[COMPARTMENT_DETECTED]);  // Atualiza o ciclo de trabalho do motor
    return "girando";  // Retorna o status do motor
}
//===========================================================================
static char *trash_Motor_Selector(){
    trash_OLED_Display(DISPLAY_SHOW[2]);  // Mostra mensagem de detecção
    sleep_ms(200);  // Aguarda 200ms
    return TRASH_COMPARTMENT_NAMES[COMPARTMENT_DETECTED];  // Retorna o nome do compartimento
}
// Funções de alarme e callback +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void Gpios_disable(){
    gpio_put(this_pins.PIN_MOTOR_CENTER, 0);  // Desliga o motor central
    for (int i = 0; i < 3; i++)gpio_put(this_pins.PIN_LEDSRGB[i], 0);  // Desliga os LEDs RGB
    gpio_put(this_pins.PIN_CAMERA, 0);  // Desliga a câmera
}
int64_t Reset_info_callback(alarm_id_t id, void *user_data) {
    ALARM_ID = 0;  // Reseta o ID do alarme
    trash_OLED_Display(DISPLAY_SHOW[0]);  // Mostra a mensagem inicial
    trash_Lights_Generate_Colors(0, 0, 0);  // Desliga os LEDs
    trash_Update_Lights();  // Atualiza os LEDs
    trash_motor_center_rotate();  // Para o motor central
    Gpios_disable();  // Desativa os GPIOs
    memset(STATUS, 0, sizeof(STATUS));  // Limpa o status
    return 0;
}
static void Callback_Buttons(uint gpio, uint32_t events){
    if(gpio == this_pins.PIN_BUTTONS[0]){
        RETRY = ERROR_DETECTED;  // Tenta novamente se houver erro
        ERROR_DETECTED = false;  // Reseta o flag de erro
    }
    if(gpio == this_pins.PIN_BUTTONS[1]){
        if(ERROR_DETECTED)add_alarm_in_ms(5, Reset_info_callback, NULL, false);  // Cancela e reseta se houver erro
        ERROR_DETECTED = false;  // Reseta o flag de erro
    }
    if(gpio == this_pins.PIN_BUTTONS[2]){
    }
}
// Funções de hibernação +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static char *trash_hibernate(){
    static uint32_t TIMER_ALARM_MS = 1000;  // Define o tempo de alarme (1 segundos)
    STATUS[3] = trash_disable_camera();  // Desativa a câmera
    COMPARTMENT_DETECTED = COMPARTMENT_DETECTED >= 0?5:COMPARTMENT_DETECTED;  // Define o compartimento padrão
    if (ALARM_ID)cancel_alarm(ALARM_ID);  // Cancela o alarme se existir
    if(COMPARTMENT_DETECTED >= 0)ALARM_ID = add_alarm_in_ms(TIMER_ALARM_MS, Reset_info_callback, NULL, false);  // Configura o alarme
    return "Hibernando";  // Retorna o status de hibernação
}
// Funções principais ##############################################################################
void trash_init(Pins pins){
    this_pins = pins;  // Inicializa os pinos
    itr_SetCallbackFunction(Callback_Buttons);  // Configura a função de callback dos botões
    itr_Interruption(this_pins.PIN_BUTTONS[0]);  // Habilita interrupção no botão 0
    itr_Interruption(this_pins.PIN_BUTTONS[1]);  // Habilita interrupção no botão 1
    itr_Interruption(this_pins.PIN_BUTTONS[2]);  // Habilita interrupção no botão 2
    trash_Lights_Generate_Colors(0, 0, 0);  // Desliga os LEDs
    trash_Update_Lights();  // Atualiza os LEDs
    trash_Lights_Indicating_Compartment();  // Indica o compartimento
    trash_OLED_Display(DISPLAY_SHOW[0]);  // Mostra a mensagem inicial
}
//============================================================================
void trash_execute_actions(){
    static bool last_status = false;  // Último status da tampa
    bool current_status = trash_LID_open_status();  // Status atual da tampa
    static bool execute_All = false;  // Flag para executar todas as ações
    if (current_status != last_status || RETRY){
        last_status = current_status;  // Atualiza o último status
        execute_All = !current_status && !execute_All?true:execute_All;  // Define se deve executar todas as ações
        sleep_ms(50);  // Aguarda 50ms
        if(execute_All && !ERROR_DETECTED){
            execute_All = false;  // Reseta o flag de execução
            RETRY = false;  // Reseta o flag de tentar novamente
            gpio_put(this_pins.PIN_LEDSRGB[1], 0);  // Desliga o LED verde
            gpio_put(this_pins.PIN_LEDSRGB[0], 0);  // Desliga o LED vermelho
            gpio_put(this_pins.PIN_LEDSRGB[2], 1);  // Liga o LED azul
            gpio_put(this_pins.PIN_CAMERA, 1);  // Liga a câmera

            STATUS[0] = trash_camera_detect();  // Detecta o objeto
            trash_OLED_Display(DISPLAY_SHOW[1]);  // Mostra o tipo e compartimento
            trash_Lights_Indicating_Compartment();  // Indica o compartimento
            if(COMPARTMENT_DETECTED >= 0){
                STATUS[1] = trash_motor_center_rotate();  // Gira o motor central
                STATUS[2] = trash_Motor_Selector();  // Libera o seletor
                trash_OLED_Display(DISPLAY_SHOW[1]);  // Mostra o tipo e compartimento
                STATUS[5] = trash_hibernate();  // Hiberna o sistema
            }
        }
        if(ERROR_DETECTED){
            trash_OLED_Display(DISPLAY_SHOW[3]);  // Mostra mensagem de erro
        }
    }
}
//============================================================================
char **trash_get_status(){
    return STATUS;  // Retorna o status do sistema
}
//######################################################################################################