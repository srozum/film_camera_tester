/*

    Light Source Result
    - print info

    Copyright (C) 2022  Serhiy Rozum

*/
#pragma once

#ifndef _RESULT_LIGHT_SOURCE_H
#define _RESULT_LIGHT_SOURCE_H

#include <Arduino.h>

class ResultLightSource : public Result
{

public:

  ResultLightSource(byte num_of_tests) : Result(num_of_tests) {};

  void print()
  {
    print_k_iso();

    char buff[20];

    float speed = 1000000 * pow(2, (selected_item.value - 6) * -1);

    LongSelectionItem target_speed = Exposure::nearest_speed(speed);

    sprintf_P(buff, PSTR("  F: 8     S: %-5s"), target_speed.label);
    LCD.setCursor(0, 2);
    LCD.print(buff);

  }
};

#endif
