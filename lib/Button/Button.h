/*
    Button - small library to read state of hardware debounced switch buttons.
    Note: Buttons state is inverted, LOW when pressed.

    Copyright (C) 2021  Serhiy Rozum

    Example:
        Button MenuBtn(BUTTON_PIN);
        MenuBtn.init();
        MenuBtn.pressed();

*/

#ifndef _BUTTON_H
#define _BUTTON_H

#include "Arduino.h"

class Button
{

public:
    Button(byte pin) : pin(pin) {}

    void init()
    {
        pinMode(pin, INPUT);
    }

    boolean pressed()
    {
        return (*portInputRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin)) ? false : true;
    }

private:
    const byte pin;

};

#endif
