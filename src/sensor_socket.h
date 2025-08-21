/*

    Header file for managing Sensors connected to a Socket
    Sensor detection is using ID resistors.

    Cannot use boundaries array, becase sensors can be registered out of order
    or mixed between sockets.
    Instead, should check if Reading is near adc_value +/-25


    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _SENSOR_SOCKET_H
#define _SENSOR_SOCKET_H

#include <Arduino.h>
#include "config.h"
#include "sensor.h"
#include "socket.h"

class SensorSocket : public Socket
{

public:
    SensorSocket(SocketID id, uint8_t id_pin) : Socket(id, id_pin){};

    Sensor *sensor()
    {
        return (connected()) ? static_cast<Sensor *>(_device) : nullptr;
    }

    Glyph sensor_glyph()
    {
        return (connected()) ? sensor()->glyph : Glyph::NONE;
    }

    uint8_t PinToBitMask(uint8_t i)
    {
        return digitalPinToBitMask(_pins[i]);
    }
    
    // even though all pins on the same port
    uint8_t PinToPort(uint8_t i)
    {
        return digitalPinToPort(_pins[i]);
    }

};

SensorSocket SocketA = {SocketID::A, SOCKET_A_ID_PIN};
SensorSocket SocketB = {SocketID::B, SOCKET_B_ID_PIN};

#endif
