/*

    Leaf Shutter Efficiency Result
    - Tt - total open time
    - To - wide open time
    - E - Efficiency
    - Te - effective speed
    - EV Error

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _RESULT_LEAF_EFFICIENCY_H
#define _RESULT_LEAF_EFFICIENCY_H

class ResultLeafEfficiency : public Result
{

public:
  ResultLeafEfficiency(uint8_t num_of_tests) : Result(num_of_tests)
  {
    stats[num_of_stats] = new Stats();
    num_of_stats++;
  };

  void add_test()
  {
    stats[0]->add_test(num_of_tests, tests_taken, measurements[0]->time());
    Result::add_test();
  };

  void print()
  {
    char buff[20];
    char buff_a[8];
    char buff_b[8];

    if (tests_taken == 0)
    {
      LCD.setCursor(0, 1);
      LCD.print(F("Tt "));
    } else if (tests_taken == 1) {
      Format::time(stats[0]->max, buff_a);
      sprintf_P(buff, PSTR("Tt%7s To "), buff_a);
      LCD.setCursor(0, 1);
      LCD.print(buff);
    } else {
      Format::time(stats[0]->max, buff_a);
      Format::time(stats[0]->min, buff_b);

      sprintf_P(buff, PSTR("Tt%7s To%7s "), buff_a, buff_b);
      LCD.setCursor(0, 1);
      LCD.print(buff);

      Format::time(stats[0]->avg, buff_a);
      Format::speed(stats[0]->avg, buff_b);

      sprintf_P(buff, PSTR("Te%7s Se  %-5s"), buff_a, buff_b);
      LCD.setCursor(0, 2);
      LCD.print(buff);

      LongSelectionItem target_speed;

      if (selected_item.value == 0)
      {
        target_speed = Exposure::nearest_speed(stats[0]->avg);
      }
      else
      {
        target_speed = selected_item;
      }

      float ev_error = Exposure::ev_error(stats[0]->avg, target_speed.value);
      Format::ev_error(ev_error, buff_a);

      float efficiency = (stats[0]->avg / stats[0]->max) * 100.0;
      dtostrf(efficiency, 4, 1, buff_b);

      sprintf_P(buff, PSTR("Er%7s Eff %s%%"), buff_a, buff_b);
      LCD.setCursor(0, 3);
      LCD.print(buff);
    }
  }
};

#endif
