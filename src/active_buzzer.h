/*
  ActiveBuzzer - small library to make beeping noises

  Copyright (C) 2021  Serhiy Rozum

  Example:
    ActiveBuzzer Buzzer(BUZZER_PIN);
    Buzzer.init();
    Buzzer.beep();

*/

#ifndef _ACTIVE_BUZZER_H
#define _ACTIVE_BUZZER_H

#include <Arduino.h>
#include "settings.h"

class ActiveBuzzer
{

private:
    const byte pin;

public:
    explicit ActiveBuzzer(byte pin) : pin(pin) {}

    void init()
    {
        pinMode(pin, OUTPUT);
    }

    bool enabled()
    {
        return (bool)Settings::Sound.value;
    }

    // More like natural Beep
    void beep(uint16_t duration = 100)
    {
        if (enabled())
        {
            digitalWrite(pin, HIGH);
            delay(duration);
            digitalWrite(pin, LOW);
        }
    }

    // Double buz, sounds like Confirmation
    void bebeep()
    {
        beep(70);
        delay(100);
        beep(50);
    }

    // Error
    void error()
    {
        beep(250);
        delay(250);
        beep(200);
    }

    // Loud alarm sound
    void alarm()
    {
        for (int i = 0; i < 3; i++)
        {
            beep(250);
            delay(300);
        }
    }
};

#endif
