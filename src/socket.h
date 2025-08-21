/*

    Header file for managing Device connected to a Socket
    Device detection is using ID resistors.

    Cannot use boundaries array, becase devices can be registered out of order
    or mixed between sockets.
    Instead, should check if Reading is near adc_value +/-25


    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _SOCKET_H
#define _SOCKET_H

#include <Arduino.h>
#include "config.h"
#include "device.h"

#define SOCKET_DEVICE_BOUNDARIES 25
#define SOCKET_DEBOUNCE_DELAY 300

constexpr int NOT_CONNECTED = -1;

enum class SocketID : uint8_t
{
    A,
    B,
    LIGHT_UNIT
};

class Socket
{

public:
    const SocketID id;

    Socket(SocketID id, uint8_t id_pin) : id(id), id_pin(id_pin), _device(nullptr){};

    void init()
    {
        // Analog inputs have pullup resistor (except Nano's A6 and A7),
        // but those vary from board to board
        // so better to rely on external 10K pullup resistor
        pinMode(id_pin, INPUT_PULLUP);
        reset_pins();
    }

    void register_pins(uint8_t pin1, uint8_t pin2, uint8_t pin3)
    {
        _pins[0] = pin1;
        _pins[1] = pin2;
        _pins[2] = pin3;
    }

    void register_device(Device *p_device)
    {
        _devices = (Device **)realloc(_devices, (_num_of_devices + 1) * sizeof(Device *));
        _devices[_num_of_devices] = p_device;
        _num_of_devices++;
    }

    void update()
    {
        _this_update_time = millis();
        int new_device = get_device();

        if (new_device != connected_device)
        {
            if (new_device == NOT_CONNECTED)
            {
                set_device(new_device);
            }
            else if (debounce(new_device))
            {
                set_device(new_device);
            }
        }
    }

    boolean connected()
    {
        return (_device != nullptr);
    }

    DeviceID device_id()
    {
        return (connected()) ? _device->id : DeviceID::NONE;
    }

    uint8_t pin(uint8_t i)
    {
        return _pins[i];
    }

    void setPin(uint8_t i, uint8_t value)
    {
        if (_device->pins[i] == OUTPUT)
        {
            digitalWrite(_pins[i], value);
        }
    }

    virtual void set_pins()
    {
        for (uint8_t i = 0; i < DEVICE_MAX_PINS; i++)
        {
            if (_device->pins[i] == NOT_IN_USE)
            {
                reset_pin(_pins[i]);
            }
            else
            {
                pinMode(_pins[i], _device->pins[i]);
                if (_device->pins[i] == OUTPUT)
                {
                    digitalWrite(_pins[i], LOW);
                }
            }
        }
    }

protected:
    const uint8_t id_pin;
    uint8_t _pins[DEVICE_MAX_PINS];

    Device *_device = nullptr;
    Device **_devices;
    uint8_t _num_of_devices = 0;

    int connected_device = NOT_CONNECTED;

    unsigned long _last_update_time = 0;
    unsigned long _this_update_time = 0;
    int _last_update_device = NOT_CONNECTED;

    void reset_pins()
    {
        for (uint8_t i = 0; i < DEVICE_MAX_PINS; i++)
        {
            reset_pin(_pins[i]);
        }
    }

    void reset_pin(uint8_t pin)
    {
        if (pin == NOT_IN_USE)
        {
            return;
        }
        else
        {
            pinMode(pin, INPUT_PULLUP);
        }
    }

    void set_device(int new_device)
    {
        connected_device = new_device;

        if (connected_device == NOT_CONNECTED)
        {
            _device = nullptr;
            reset_pins();
        }
        else
        {
            _device = _devices[connected_device];
            set_pins();
        }
    }

    int get_device()
    {
        int value = analogRead(id_pin);
        for (int i = 0; i < _num_of_devices; i++)
        {
            if (_devices[i]->adc_value > (value - SOCKET_DEVICE_BOUNDARIES) && _devices[i]->adc_value < (value + SOCKET_DEVICE_BOUNDARIES))
                return i;
        }
        return NOT_CONNECTED;
    }

    boolean debounce(int new_device)
    {
        if (new_device != _last_update_device)
        {
            _last_update_time = _this_update_time;
        }
        _last_update_device = new_device;
        return (_this_update_time - _last_update_time > SOCKET_DEBOUNCE_DELAY);
    }
};

#endif
