#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
void buzzer_control(uint pino_Buzzer, bool ativar);
void buzzer_play_note(uint pino_Buzzer, int frequencia, int duracao);
void buzzer_multiplay(uint pino_Buzzer, int *notas, int *duracoes, int tamanho);

#endif 