/*
    Film Camera Tester - Exposure Math
    Copyright (C) 2021  Serhiy Rozum
*/

#pragma once

#include <Arduino.h>
#include "config.h"

namespace Exposure
{

  // Time in microseconds
  // Divided by 2 because Timer2 counts in half microseconds
  float time(float start, float stop)
  {
    return (stop - start) / 2.0;
  }

  float ev_error(float t_measured, float t_target)
  {
    return log(t_measured / t_target) / log(2.0);
  }

  // Detect nearest target speed
  LongSelectionItem nearest_speed(float time)
  {
    const LongSelectionItem *speeds;
    uint8_t speeds_count;

    if (Settings::Speeds.value == 0)
    {
      speeds = SelectionItems::SpeedsListOld;
      speeds_count = SIZEOF(SelectionItems::SpeedsListOld);
    }
    else if (Settings::Speeds.value == 1)
    {
      speeds = SelectionItems::SpeedsListNew;
      speeds_count = SIZEOF(SelectionItems::SpeedsListNew);
    }
    else
    {
      speeds = SelectionItems::SpeedsListLog;
      speeds_count = SIZEOF(SelectionItems::SpeedsListLog);
    }

    // Skip 0 which is "Auto" and start from 8s
    uint8_t bestmatch = 1;
    float besterror = 800000.0; // 10ms
    float error;
    LongSelectionItem speed;

    for (uint8_t i = bestmatch; i < speeds_count; i++)
    {

      memcpy_P(&speed, &speeds[i], sizeof(LongSelectionItem));

      error = abs(time - speed.value);

      if (error < besterror)
      {
        besterror = error;
        bestmatch = i;
      }
    }

    memcpy_P(&speed, &speeds[bestmatch], sizeof(LongSelectionItem));

    return speed;
  }

}
