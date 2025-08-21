/*

    RotaryEncoder - library to read rotary encoder switch

    Copyright (C) 2021  Serhiy Rozum

*/

#ifndef _ROTARY_ENCODER_H
#define _ROTARY_ENCODER_H

#include <Arduino.h>

class RotaryEncoder
{

private:
    const uint8_t pinCLK;
    const uint8_t pinDA;
    volatile int8_t delta;
    volatile int8_t last;

public:
    explicit RotaryEncoder(uint8_t CLK, uint8_t DA);

    void init();
    void start();
    void stop();
    void update();
    int8_t getValue();
    int8_t read();
};

#endif
