/*

    Header file for managing Light Unit connected to a Socket
    Sensor detection is using ID resistors.

    Cannot use boundaries array, becase sensors can be registered out of order
    or mixed between sockets.
    Instead, should check if Reading is near adc_value +/-25


    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _LIGHT_UNIT_SOCKET_H
#define _LIGHT_UNIT_SOCKET_H

#include <Arduino.h>
#include "config.h"
#include "light_unit.h"
#include "socket.h"

class LightUnitSocket : public Socket
{

public:
    LightUnitSocket(SocketID id, uint8_t id_pin) : Socket(id, id_pin){};

    LightUnit *unit()
    {
        return (connected()) ? static_cast<LightUnit *>(_device) : nullptr;
    }

    void on()
    {
        if (connected())
        {
            fanOn();
            delay(200);
            lightOn();
        }
    }

    void off()
    {
        if (connected())
        {
            lightOff();
            delay(200);
            fanOff();
        }
    }

    void setEV(byte k, byte selected_ev)
    {
        EVStop ev_stop;
        memcpy_P(&ev_stop, &unit()->ev_tables[k].ev_stops[selected_ev], sizeof(EVStop));

        TCCR1A &= ~_BV(COM1A1); // Stop

        ICR1 = ev_stop.counter_top;
        OCR1A = ev_stop.duty;
        TCNT1 = 0; // Reset to 0 in case when resolution is lowered, OCRA may miss the cycle

        TCCR1A |= _BV(COM1A1); // Start
    }

    void lightOn()
    {
        setPin(0, HIGH);
    }

    void lightOff()
    {
        setPin(0, LOW); // uses digitalWrite method, which stops PWM
    }

    void fanOn()
    {
        setPin(1, HIGH);
    }

    void fanOff()
    {
        setPin(1, LOW);
    }

    void set_pins()
    {
        Socket::set_pins();

        TCCR1A = _BV(WGM11);                          /* mode 14: fast PWM, TOP=ICR1 */
        TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10); /* no prescaling */
    };
};

LightUnitSocket LightSource = {SocketID::LIGHT_UNIT, LIGHT_UNIT_ID_PIN};

#endif
