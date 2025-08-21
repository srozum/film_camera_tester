/*

    Simple or Series Result
    - time
    - speed
    - target speed (set or automatically detected)
    - EV error compared to target speed

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _RESULT_SIMPLE_H
#define _RESULT_SIMPLE_H

#include <result.h>

class ResultSimple : public Result
{

public:
  LongSelectionItem target_speed;

  ResultSimple(uint8_t num_of_tests) : Result(num_of_tests)
  {
    stats[num_of_stats] = new Stats();
    num_of_stats++;
  };

  void add_test()
  {
    float T = measurements[0]->time() + CENTER_CORRECTION;

    if (T > RESULT_MAX_MEASURED_TIME)
      return;

    stats[0]->add_test(num_of_tests, tests_taken, T);

    if (tests_taken == 0)
    {
      if (selected_item.value == 0)
      {
        target_speed = Exposure::nearest_speed(T);
      }
      else
      {
        target_speed = selected_item;
      }
    }

    Result::add_test();
  };

  void print()
  {
    if (tests_taken > 0)
    {

      float T = measurements[0]->time() + CENTER_CORRECTION;

      if (T > RESULT_MAX_MEASURED_TIME)
      {
        print_error();
      }
      else
      {

        if (num_of_tests > 1)
        {
          // Series mode
          if (finished())
          {
            print_series_summary();
          }
          else
          {
            print_time(T);
            print_ev_error(T);
            print_tests_counter();
          }
        }
        else {
            print_time(T);
            print_ev_error(T);

        }
      }
    }
    else
    {
      print_running();
    }
  };

  void print_ev_error(float &time)
  {
    char buff[20];
    char buff_a[8];

    float ev_error = Exposure::ev_error(time, target_speed.value);

    Format::ev_error(ev_error, buff_a);

    sprintf_P(buff, PSTR("Er %7s of %-5s "), buff_a, target_speed.label);
    LCD.setCursor(0, 2);
    LCD.print(buff);
  }

  void print_series_summary()
  {
    char buff[20];
    char buff_a[8];
    char buff_b[8];

    Format::time(stats[0]->min, buff_a);
    Format::speed(stats[0]->avg, buff_b);

    sprintf_P(buff, PSTR("Mn %7s S: %-5s "), buff_a, buff_b);
    LCD.setCursor(0, 1);
    LCD.print(buff);

    float avg_error = Exposure::ev_error(stats[0]->avg, target_speed.value);

    Format::time(stats[0]->avg, buff_a);
    Format::ev_error(avg_error, buff_b);

    sprintf_P(buff, PSTR("Av %7s Er%-7s"), buff_a, buff_b);
    LCD.setCursor(0, 2);
    LCD.print(buff);

    Format::time(stats[0]->max, buff_a);

    sprintf_P(buff, PSTR("Mx %7s of %-5s "), buff_a, target_speed.label);
    LCD.setCursor(0, 3);
    LCD.print(buff);
  };
};

#endif
