/*

    Focal Plane Shutter Result
    - times at three points of frame A/B/C
    - speed at the center
    - target speed (set or automatically detected)
    - EV error compared to target speed
    - curtains travel time To/Tc

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _RESULT_FOCAL_H
#define _RESULT_FOCAL_H

#define EDGES_CORRECTION (12.25)
#define OVERLAP_COMPENSATION (0.17)
#define OPENING_CURTAIN_CORRECTION (29.0)
#define CLOSING_CURTAIN_CORRECTION (-17.0)

class ResultFocal : public Result
{

public:

  ResultFocal(uint8_t num_of_tests) : Result(num_of_tests) {
    // To
    stats[num_of_stats] = new Stats();
    num_of_stats++;
    // Tc
    stats[num_of_stats] = new Stats();
    num_of_stats++;
  };

  void add_test()
  {
    if (selected_item.value == 3)
    {

      float Toc = measurements[3]->time() * frame_ratio();
      stats[0]->add_test(num_of_tests, tests_taken, Toc);

      float Tcc = measurements[4]->time() * frame_ratio();
      stats[1]->add_test(num_of_tests, tests_taken, Tcc);

    }
    Result::add_test();
  };

  void print()
  {
    if (finished())
    {
      if (measurements[1]->time() > RESULT_MAX_MEASURED_TIME)
      {
        print_error();
      }
      else
      {
        switch (selected_item.value)
        {
        case 0: // Time mode
          print_focal_time();
          break;
        case 1: // Exposure mode
          print_focal_exposure();
          break;
        case 2: // Curtains travel time mode
          print_focal_curtains_ab();
          break;
        case 3: // Average curtains travel time
          print_focal_average();
          break;
        }
      }
    }
    else if (tests_taken > 0)
    {
      if (measurements[1]->time() > RESULT_MAX_MEASURED_TIME)
      {
        print_error();
      }
      else
      {
        print_focal_curtains();
        print_tests_counter();
      }
    }
    else
    {
      print_running();
    }
  }

  void print_focal_time()
  {

    float Toc = measurements[3]->time() * frame_ratio();
    float Tcc = measurements[4]->time() * frame_ratio();

    float Ts = ((Toc + Tcc) * OVERLAP_COMPENSATION) / (2.0 * frame_space);

    float Ta = measurements[0]->time() + EDGES_CORRECTION + Ts;
    float Tb = measurements[1]->time();
    float Tc = measurements[2]->time() + EDGES_CORRECTION + Ts;

    char buff[20];
    char buff_a[8];
    char buff_b[8];

    Format::time(Ta, buff_a);
    Format::speed(Tb, buff_b);

    sprintf_P(buff, PSTR("A %7s S: %-5s "), buff_a, buff_b);
    LCD.setCursor(0, 1);
    LCD.print(buff);

    Format::time(Tb, buff_a);
    Format::time(Toc, buff_b);

    sprintf_P(buff, PSTR("B %7s To %-7s"), buff_a, buff_b);
    LCD.setCursor(0, 2);
    LCD.print(buff);

    Format::time(Tc, buff_a);
    Format::time(Tcc, buff_b);

    sprintf_P(buff, PSTR("C %7s Tc %-7s"), buff_a, buff_b);
    LCD.setCursor(0, 3);
    LCD.print(buff);
  }

  void print_focal_exposure()
  {
    float Ts = ((measurements[3]->time() + measurements[4]->time()) * OVERLAP_COMPENSATION) / (2.0 * frame_space);

    float Ta = measurements[0]->time() + EDGES_CORRECTION + Ts;
    float Tb = measurements[1]->time();
    float Tc = measurements[2]->time() + EDGES_CORRECTION + Ts;

    LongSelectionItem target_speed = Exposure::nearest_speed(Tb);

    float ev_error;

    char buff[20];
    char buff_a[8];
    char buff_b[8];

    ev_error = Exposure::ev_error(Ta, Tb);
    Format::ev_error(ev_error, buff_a, 2);
    Format::speed(Tb, buff_b);

    sprintf_P(buff, PSTR("A %7s S: %-5s "), buff_a, buff_b);
    LCD.setCursor(0, 1);
    LCD.print(buff);

    Format::time(Tb, buff_a);
    ev_error = Exposure::ev_error(Tb, target_speed.value);
    Format::ev_error(ev_error, buff_b);

    sprintf_P(buff, PSTR("B %7s Er %-7s"), buff_a, buff_b);
    LCD.setCursor(0, 2);
    LCD.print(buff);

    ev_error = Exposure::ev_error(Tc, Tb);
    Format::ev_error(ev_error, buff_a, 2);

    sprintf_P(buff, PSTR("C %7s of %-5s "), buff_a, target_speed.label);
    LCD.setCursor(0, 3);
    LCD.print(buff);
  }

  void print_focal_curtains_ab()
  {
    char buff[20];
    char buff_a[8];
    char buff_b[8];

    float Toc = measurements[3]->time() * frame_ratio();
    float Tcc = measurements[4]->time() * frame_ratio();

    float Toa = (Exposure::time(measurements[0]->start, measurements[1]->start) + OPENING_CURTAIN_CORRECTION) * frame_ratio();
    float Tca = (Exposure::time(measurements[0]->stop, measurements[1]->stop) + CLOSING_CURTAIN_CORRECTION) * frame_ratio();

    Format::time(Toa, buff_a);
    Format::time(Tca, buff_b);

    sprintf_P(buff, PSTR("AB %7s   %7s"), buff_a, buff_b);
    LCD.setCursor(0, 1);
    LCD.print(buff);

    float Tob = (Exposure::time(measurements[1]->start, measurements[2]->start) + OPENING_CURTAIN_CORRECTION) * frame_ratio();
    float Tcb = (Exposure::time(measurements[1]->stop, measurements[2]->stop) + CLOSING_CURTAIN_CORRECTION) * frame_ratio();


    Format::time(Tob, buff_a);
    Format::time(Tcb, buff_b);

    sprintf_P(buff, PSTR("BC %7s   %7s"), buff_a, buff_b);
    LCD.setCursor(0, 2);
    LCD.print(buff);

    Format::time(Toc, buff_a);
    Format::time(Tcc, buff_b);

    sprintf_P(buff, PSTR("To %7s Tc%7s"), buff_a, buff_b);
    LCD.setCursor(0, 3);
    LCD.print(buff);
  }

  // Intermediate result for curtains average time
  void print_focal_curtains()
  {
    char buff[21];
    char buff_a[8];
    char buff_b[8];

    float Toc = measurements[3]->time() * frame_ratio();
    Format::time(Toc, buff_a);

    float Tcc = measurements[4]->time() * frame_ratio();
    Format::time(Tcc, buff_b);

    sprintf_P(buff, PSTR("To %7s Tc%7s"), buff_a, buff_b);
    LCD.setCursor(0, 1);
    LCD.print(buff);
  }

  void print_focal_average()
  {
    char buff[20];
    char buff_a[8];
    char buff_b[8];

    Format::time(stats[0]->min, buff_a);
    Format::time(stats[1]->min, buff_b);

    sprintf_P(buff, PSTR("Mn %7s   %7s"), buff_a, buff_b);
    LCD.setCursor(0, 1);
    LCD.print(buff);

    Format::time(stats[0]->avg, buff_a);
    Format::time(stats[1]->avg, buff_b);

    sprintf_P(buff, PSTR("Av %7s   %7s"), buff_a, buff_b);
    LCD.setCursor(0, 2);
    LCD.print(buff);

    Format::time(stats[0]->max, buff_a);
    Format::time(stats[1]->max, buff_b);

    sprintf_P(buff, PSTR("Mx %7s   %7s"), buff_a, buff_b);
    LCD.setCursor(0, 3);
    LCD.print(buff);
  }

  float frame_ratio()
  {
    return frame_space / sensor_space;
  }

};

#endif
