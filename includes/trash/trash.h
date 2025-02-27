#ifndef THASH_H
#define THASH_H
    #include <pico/stdlib.h>
    typedef struct {
        uint PIN_TRASH_LID;  // Pino PWM
        uint PIN_TRASH_LEDS;  // Pino de direção
        uint PIN_MOTOR_CENTER; // Pino de habilitação
        uint PIN_OLED_SDA;  // Pino SDA do display OLED
        uint PIN_OLED_SCL;  // Pino SCL do display OLED
        uint PIN_CAMERA;
        uint PIN_BUZZER;
        uint PIN_BUTTONS[3];
        uint PIN_LEDSRGB[3];
    } Pins;
    void trash_init(Pins pins);
    void trash_execute_actions();
    char **trash_get_status();
#endif
