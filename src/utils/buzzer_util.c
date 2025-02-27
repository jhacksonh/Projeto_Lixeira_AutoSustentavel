#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "buzzer_util.h"

static int SLICE_NUM  = 0;

void buzzer_control(uint pino_Buzzer, bool ativar) {
    if (ativar) {
        gpio_set_function(pino_Buzzer, GPIO_FUNC_PWM); // Configura o pino como PWM
    } else {
        gpio_set_function(pino_Buzzer, GPIO_FUNC_SIO); // Configura o pino como GPIO
        gpio_set_dir(pino_Buzzer, GPIO_OUT); // Define como saída
        gpio_put(pino_Buzzer, 0); // Define o pino como LOW
    }
    pwm_set_enabled(SLICE_NUM, ativar);
}

void buzzer_play_note(uint pino_Buzzer, int frequencia, int duracao) {
    if (frequencia > 0 && duracao > 0) { // Verifica frequência e duração válidas
        SLICE_NUM = pwm_gpio_to_slice_num(pino_Buzzer);
        int channel = pwm_gpio_to_channel(pino_Buzzer);
        uint32_t freq_sistema = clock_get_hz(clk_sys); // Frequência do sistema
        uint16_t wrap_valor = freq_sistema / frequencia - 1; // Define o valor de wrap
        pwm_set_wrap(SLICE_NUM, wrap_valor);
        pwm_set_chan_level(SLICE_NUM, channel, wrap_valor / 2); // Duty cycle de 50%
        buzzer_control(pino_Buzzer, true); // Configura o pino como PWM
        sleep_ms(duracao); // Duração da nota
        buzzer_control(pino_Buzzer, false); // Configura o pino como GPIO e define como LOW
    }
}

void buzzer_multiplay(uint pino_Buzzer, int *notas, int *duracoes, int tamanho) {
    if (notas && duracoes && tamanho > 0) { // Verifica arrays válidos
        for (int i = 0; i < tamanho; i++) {
            int frequencia = notas[i];
            int duracao = duracoes[i];
            buzzer_play_note(pino_Buzzer, frequencia, duracao);
            sleep_ms(10); // Pequeno delay entre as notas
        }
    }
}