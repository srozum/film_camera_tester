/*

    Timer2 Library to increase timer resolution up to 0.5µs

    Copyright (C) 2021  Serhiy Rozum

*/

#ifndef _TIMER2_H
#define _TIMER2_H

#include <Arduino.h>

class Timer2
{
public:

  volatile unsigned long overflow_count;

  struct Count
  {
    unsigned long overflow_count;
    uint8_t count;
    unsigned long total_count();
  };

  Timer2();

  void start();
  void stop();
  void reset();
  void get_count(Timer2::Count *count);
  void increment_overflow_count();

private:
  byte _tccr2a_save;
  byte _tccr2b_save;
};

#endif
