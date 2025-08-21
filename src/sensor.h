/*

    Header file for describing supported Sensors

    Sensor detection is based on a resistive button array approach

    Hardware:
    Common resistor 2.2K plus 10K pullup resistor (Nano analog inputs A6/A7 don't have internal pullups).
    When sensor not connected, analog input should read 1024

    0 - no resistor, shorted, not in use
    1 - Sensor #1 	366	1K    - FP Sensor V1 32x22
    2 - Sensor #1   407	1.2K  - FP Sensor V2 32x20
    3 - Sensor #1 	463	1.5K  - TODO: FP V3 Sensor 645
    4 - Sensor #2		535	2K    - Dot
    5 - Sensor #3		608	2.7K  - Dot with laser
    6 - Sensor #4		635	3K    - TODO: 2 Dot with LED / Leica Sensor
    7 - Sensor #5		696	3.9K
    8 - Sensor #6		734	4.7K
    9 - Sensor #7	  803	6.8K  - Flash sync
    10- Sensor #8		850	9.1K  - TODO: Light Sensor

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _SENSOR_H
#define _SENSOR_H

#include <Arduino.h>
#include "config.h"
#include "glyphs.h"
#include "device.h"

class Sensor : public Device
{
public:
  const Glyph glyph;

  Sensor(DeviceID id, int adc_value, Glyph glyph) : Device(id, adc_value), glyph(glyph) {};

};

Sensor Sensor1 = {DeviceID::SENSOR_FP1, 366, Glyph::FOCAL}; // 32x22
Sensor Sensor2 = {DeviceID::SENSOR_FP2, 407, Glyph::FOCAL}; // 32x20
Sensor Sensor4 = {DeviceID::SENSOR_DOT, 535, Glyph::DOT};
Sensor Sensor5 = {DeviceID::SENSOR_LASER, 608, Glyph::LASER};
Sensor Sensor6 = {DeviceID::SENSOR_EXP, 700, Glyph::EXPOSURE};
Sensor Sensor7 = {DeviceID::SENSOR_FLASH, 803, Glyph::FLASH};

#endif
