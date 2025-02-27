#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "adc_util.h"
#include "oled_util.h"
#include "interrupt_util.h"
#include "buzzer_util.h"
#include "leds_util.h"
#include "pwm_util.h"
#include "trash.h"

#define PIN_TRASH_LID 26
#define PIN_TRASH_LEDS 7
#define PIN_OLED_SDA 14
#define PIN_OLED_SCL 15
#define PIN_MOTOR_CENTER 4
#define PIN_MOTOR_SELETOR 8
#define PIN_CAMERA 9
#define PIN_BUZZER 21

static uint PINS_SENSORS[5] = {16,17,18,19,20};
static uint PINS_BUTTONS[3] = {5,6,22};
static uint PINS_LEDSRGB[3] = {13,11,12};
static const float WRAP_PWM = 250000;

Pins pins = {PIN_TRASH_LID, PIN_TRASH_LEDS, PIN_MOTOR_CENTER, PIN_OLED_SDA, PIN_OLED_SCL, PIN_CAMERA, PIN_BUZZER};

void config_pins(uint pin, bool output, bool pull_up){
    gpio_init(pin);
    gpio_set_dir(pin, output);
    if (pull_up)gpio_pull_up(pin);
    if(!pull_up)gpio_put(pin, 0);
}
void initialize_All() {
    for (int i = 0; i < 3; i++) {
        pins.PIN_BUTTONS[i] = PINS_BUTTONS[i];
        pins.PIN_LEDSRGB[i] = PINS_LEDSRGB[i];
        config_pins(pins.PIN_BUTTONS[i], GPIO_IN, true);
        config_pins(pins.PIN_LEDSRGB[i], GPIO_OUT, false);
    }
    stdio_init_all();
    gpio_set_function(PIN_BUZZER, GPIO_FUNC_PWM);
    setup_adc(PIN_TRASH_LID);
    ws2812_init(PIN_TRASH_LEDS,25);
    oled_Init(PIN_OLED_SDA, PIN_OLED_SCL);
    setup_pwm(PIN_MOTOR_CENTER, WRAP_PWM);
    trash_init(pins);
}
bool reverse = false;
int main(){
    initialize_All();
    int i = 0;
    while (true){
        trash_execute_actions();
        sleep_ms(100);
    }
}
