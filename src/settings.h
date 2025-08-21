/*

    EEPROM Settings

    Copyright (C) 2021  Serhiy Rozum

*/

#pragma once

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>

#define START_ADDRESS 4

class SettingBase
{
  protected:
      static int new_addr;
};

int SettingBase::new_addr = START_ADDRESS;

template <typename T> 
class Setting: private SettingBase
{

public:
    int addr;
    T value;

    Setting()
    {
        addr = new_addr;
        new_addr += sizeof(T);
        EEPROM.get(addr, value);
    }

    void update(T v)
    {
        value = v;
        EEPROM.put(addr, value);
    }

};


namespace Settings
{
    // address 0 skipped
    Setting<byte> Mode;
    Setting<byte> Sound;
    Setting<byte> Speeds;
    Setting<byte> Series;
    Setting<byte> FrameWidth;
    Setting<byte> FrameHeight;
    Setting<byte> KValue;
    Setting<unsigned long> K12Exposure;
    Setting<unsigned long> K14Exposure;
    Setting<unsigned long> K12ShiftedExposure;
    Setting<unsigned long> K14ShiftedExposure;
    Setting<byte> EVShift;

}

#endif
