/*

    Flash Sync Result
    - print standadr exposure time line
    - detect X or M delay
    format and print accordingly

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _RESULT_FLASH_SYNC_H
#define _RESULT_FLASH_SYNC_H

#include <Arduino.h>

class ResultFlashSync : public Result
{

public:
  ResultFlashSync(uint8_t num_of_tests) : Result(num_of_tests){};

  void print()
  {
    if (finished())
    {

      float T = measurements[0]->time() + CENTER_CORRECTION;

      if (T > RESULT_MAX_MEASURED_TIME)
      {
        print_error();
      }
      else
      {
        print_time(T);
        print_flash_sync();
      }
    }
    else
    {
      print_running();
    }
  };

  void print_flash_sync()
  {
    char buff[20];
    char buff_a[8];
    char buff_b[8];

    if (num_of_measurements == 2)
    {
      // Simple
      float D = measurements[1]->time();

      char delayChar = (D > 0) ? 'M' : 'X';

      Format::time(D, buff_a);

      sprintf_P(buff, PSTR("%c: %7s"), delayChar, buff_a);
      LCD.setCursor(0, 2);
      LCD.print(buff);
    }
    else
    {
      // Focal
      if (measurements[3]->time() > 0)
      {
        // FP
        float FP = measurements[3]->time();

        Format::time(FP, buff_a);

        sprintf_P(buff, PSTR("FP: %7s"), buff_a);
        LCD.setCursor(0, 2);
        LCD.print(buff);
      }
      else
      {
        // X
        float X = measurements[1]->time();
        float D = measurements[2]->time();

        Format::time(X, buff_a);

        sprintf_P(buff, PSTR("+X: %7s"), buff_a);
        LCD.setCursor(0, 2);
        LCD.print(buff);

        Format::time(D, buff_b);

        sprintf_P(buff, PSTR("XD: %7s"), buff_b);
        LCD.setCursor(0, 3);
        LCD.print(buff);
      }
    }
  }
};

#endif
