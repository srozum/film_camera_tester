/*

    Auto Exposure Result
    - print EV error and Speed

    Copyright (C) 2022  Serhiy Rozum

*/
#pragma once

#ifndef _RESULT_EXPOSURE_H
#define _RESULT_EXPOSURE_H

#include <Arduino.h>

#define EXPOSURE_COUNTS_MIN 5   // Minimum counts to display, 2.5 stops below calibrated value
#define EXPOSURE_COUNTS_MAX 700 // Maxumim counts to display, 2.5 stops above calibrated value

class ResultExposure : public Result
{

public:
  ResultExposure(byte num_of_tests) : Result(num_of_tests){};

  void print()
  {
    print_k_iso();

    char buff[20];
    char error[8] = "- - - -";
    char speed[8] = "";

    float counts = Counter.counts + 1;
    float time = Counter.time();

    if ((counts > EXPOSURE_COUNTS_MIN) && (counts < EXPOSURE_COUNTS_MAX) && (time < RESULT_MAX_MEASURED_TIME))
    {

      counts = counts * 128;

      unsigned long calibration_value;

      if (Settings::KValue.value == 0)
      {
        calibration_value = (Settings::EVShift.value == 0) ? Settings::K12Exposure.value : Settings::K12ShiftedExposure.value;
      }
      else
      {
        calibration_value = (Settings::EVShift.value == 0) ? Settings::K14Exposure.value : Settings::K14ShiftedExposure.value;
      }

      float ev_error = Exposure::ev_error(counts, calibration_value);
      Format::ev_error(ev_error, error, 2);

      Format::speed(time, speed);
    }

    LCD.setCursor(4, 2);
    sprintf_P(buff, PSTR("Exp: %7s  "), error);
    LCD.print(buff);

    LCD.setCursor(6, 3);
    sprintf_P(buff, PSTR("S: %-6s  "), speed);
    LCD.print(buff);
  }
};

#endif
