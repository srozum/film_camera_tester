/*

    Header file for describing supported Light Units

    Sensor detection is based on a resistive button array approach

    Hardware:
    Common resistor 2.2K plus 10K pullup resistor (Nano analog inputs A6/A7 don't have internal pullups).
    When device not connected, analog input should read 1024

    Light Units
    0 - no resistor, shorted, not in use
    1 - Light Unit #1   366  1K    - LU V1 On/Off, PWM not guaranteed
    2 - Light Unit #2   535  2K    -
    3 - Light Unit #3   635  3K    -

    Copyright (C) 2021  Serhiy Rozum


*/
#pragma once

#ifndef _LIGHT_UNIT_H
#define _LIGHT_UNIT_H

#include <Arduino.h>
#include "config.h"
#include "device.h"
#include "selection_items.h"

typedef struct
{
  byte value; // EV Stop, called 'value' to use search function
  uint16_t counter_top;
  uint16_t duty;
} EVStop;

class LightUnit : public Device
{
public:
  struct EVStopsTable
  {
    const EVStop *ev_stops;
    byte num_of_ev_stops;
  };

  EVStopsTable *ev_tables;
  uint8_t num_of_ev_tables = 0;

  LightUnit(DeviceID id, int adc_value) : Device(id, adc_value){};

  template <byte N>
  void add_ev_table(const EVStop (&ev_stops)[N])
  {
    EVStopsTable ev_table = {ev_stops, N};

    ev_tables = (EVStopsTable *)realloc(ev_tables, (num_of_ev_tables + 1) * sizeof(ev_table));
    ev_tables[num_of_ev_tables] = ev_table;
    num_of_ev_tables++;
  }

};

// LightUnit LightUnit1 = {DeviceID::LIGHT_UNIT_1, 366, false};
LightUnit LightUnit2 = {DeviceID::LIGHT_UNIT_2, 510};


const EVStop LightUnit2K12EV[] PROGMEM = {
    {5, 32374, 70},
    {6, 17060, 70},
    {7, 8872, 70},
    {8, 4562, 70},
    {9, 10849, 140},
    {10, 5439, 140},
    {11, 2690, 140},
    {12, 6063, 300},
    {13, 2903, 300},
    {14, 1351, 300},
    {15, 3599, 800},
    {16, 1429, 800},
    {17, 2036, 2012},
};

const EVStop LightUnit2K14EV[] PROGMEM  = {
    {5, 29108, 70},
    {6, 15314, 70},
    {7, 7947, 70},
    {8, 4080, 70},
    {9, 9715, 140},
    {10, 4857, 140},
    {11, 2401, 140},
    {12, 5407, 300},
    {13, 2572, 300},
    {14, 1188, 300},
    {15, 3121, 800},
    {16, 1202, 800},
    {17, 2041, 2040},
};


#endif
