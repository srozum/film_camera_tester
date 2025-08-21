/*

    Header file for describing Devices

    Sensor detection is based on a resistive button array approach

    Hardware:
    Common resistor 2.2K plus 10K pullup resistor (Nano analog inputs A6/A7 don't have internal pullups).
    When sensor not connected, analog input should read 1024

    0 - no resistor, shorted, not in use
    1 - Sensor #1   366  1K    - FP Sensor V1 32x22 (PCB 32x22, but in fact openings 33x21!)
    2 - Sensor #1   407  1.2K  - FP Sensor V2 32x20
    3 - Sensor #1   463  1.5K  - TODO: FP V3 Sensor 645
    4 - Sensor #2   535  2K    - Dot
    5 - Sensor #3  608  2.7K  - Dot with laser
    6 - Sensor #4  635  3K    - TODO: 2 Dot with LED / Leica Sensor
    7 - Sensor #5  696  3.9K
    8 - Sensor #6  734  4.7K
    9 - Sensor #7  803  6.8K  - Flash sync
    10- Sensor #8  850  9.1K  - TODO: Light Sensor

    Light Units
    0 - no resistor, shorted, not in use
    1 - Light Unit #1   366  1K    - LU V1 On/Off, PWM not guaranteed
    2 - Light Unit #2   535  2K    - LU V2 Calibrated
    3 - Light Unit #3   635  3K    -

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _DEVICE_H
#define _DEVICE_H

#include <Arduino.h>
#include "config.h"

#define DEVICE_MAX_PINS 3

enum class DeviceID : uint8_t
{
    NONE,
    SENSOR_FP1,
    SENSOR_FP2,
    SENSOR_DOT,
    SENSOR_LASER,
    SENSOR_FLASH,
    SENSOR_EXP,
    LIGHT_UNIT_2,
};

class Device
{
public:
    const DeviceID id;
    const int adc_value;
    uint8_t pins[DEVICE_MAX_PINS];

    Device(DeviceID id, int adc_value) : id(id), adc_value(adc_value){};

    void register_pins(uint8_t pin1, uint8_t pin2, uint8_t pin3)
    {
        pins[0] = pin1;
        pins[1] = pin2;
        pins[2] = pin3;
    }

};

#endif
