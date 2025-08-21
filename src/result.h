/*

    Mode Result
    - hold measurements
    - format and print measurements

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _RESULT_H
#define _RESULT_H

#include <Arduino.h>

#define RESULT_MAX_MEASUREMENTS 5          // Max in Focal Flash mode Exp x3 + Flash x2 = 5
#define RESULT_MAX_STATS 2                 // Max in Focal Avg mode
#define RESULT_MAX_MEASURED_TIME 10000000L // 10s

#define CENTER_CORRECTION (7.87)

class Measurement
{
public:
  unsigned long start;
  unsigned long stop;

  Measurement(unsigned long start, unsigned long stop) : start(start), stop(stop){};

  float time()
  {
    return Exposure::time(start, stop);
  };
};

class Stats
{
public:
  float min = 0;
  float max = 0;
  float avg = 0;

  Stats(){};

  void add_test(uint8_t num_of_tests, uint8_t tests_taken, float value)
  {
    // first test automatically resets stats
    if (tests_taken == 0)
    {
      min = value;
      max = value;
      avg = (float)value / num_of_tests;
    }
    else
    {
      if (value < min)
        min = value;
      if (value > max)
        max = value;
      avg += (float)value / num_of_tests;
    }
  };
};

class Result
{

public:
  Measurement *measurements[RESULT_MAX_MEASUREMENTS];
  uint8_t num_of_measurements = 0;

  Stats *stats[RESULT_MAX_STATS];
  uint8_t num_of_stats = 0;

  uint8_t num_of_tests;
  uint8_t tests_taken = 0;

  // for ResultFocal
  float frame_space;
  float sensor_space;

  LongSelectionItem selected_item;

  Result(uint8_t num_of_tests) : num_of_tests(num_of_tests){};

  virtual ~Result()
  {
    clear_measurements();
    clear_stats();
  }

  void add_measurement(unsigned long start, unsigned long stop)
  {
    measurements[num_of_measurements] = new Measurement(start, stop);
    num_of_measurements++;
  }

  virtual void add_test()
  {
    tests_taken++;
  };

  bool finished()
  {
    return tests_taken == num_of_tests;
  }

  // Result output methods

  virtual void print(){};

  void print_time(float &time, char row = 1)
  {
    char buff[20];
    char buff_a[8];
    char buff_b[8];

    Format::time(time, buff_a);
    Format::speed(time, buff_b);

    sprintf_P(buff, PSTR("T: %7s S: %-5s "), buff_a, buff_b);
    LCD.setCursor(0, row);
    LCD.print(buff);
  }

  void print_tests_counter()
  {
    if (num_of_tests > 1)
    {
      char buff[20];
      sprintf_P(buff, PSTR("Test %2d of %2d"), tests_taken, num_of_tests);
      LCD.setCursor(0, 3);
      LCD.print(buff);
    }
  }

  void print_k_iso() {
    char buff[21];

    sprintf_P(buff, PSTR("ISO: 100   K: %-5s%-1s"), (Settings::KValue.value ? "14.03" : "12.50" ), (Settings::EVShift.value ? "\x4" : ""));

    LCD.setCursor(0, 1);
    LCD.print(buff);
  }

  void print_running()
  {
    LCD.setCursor(4, 2);
    LCD.print(F("Running..."));
  }

  void print_error()
  {
    LCD.setCursor(4, 2);
    LCD.print(F("Error"));
  }

  void print_calibration()
  {
    LCD.setCursor(3, 2);
    LCD.print(F("Calibrating..."));
  }

  virtual void reset()
  {
    tests_taken = 0;
    clear_measurements();
  }

  // Destructor methods

  void clear_measurements()
  {
    for (uint8_t i = 0; i < num_of_measurements; i++)
    {
      delete measurements[i];
      measurements[i] = NULL;
    }
    num_of_measurements = 0;
  }

  void clear_stats()
  {
    for (uint8_t i = 0; i < num_of_stats; i++)
    {
      delete stats[i];
      stats[i] = NULL;
    }
    num_of_stats = 0;
  }
};

#endif
