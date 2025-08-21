/*

    Mode Exposure

    Copyright (C) 2022  Serhiy Rozum

*/
#pragma once

#ifndef _MODE_EXPOSURE_H
#define _MODE_EXPOSURE_H

#include "exposure_counter.h"

#define EV12_INDEX 7
#define EV13_INDEX 8

class ModeExposure : public Mode
{

public:
  template <byte N>
  ModeExposure(Requirement (&requirements)[N], LightUnitSocket *light_source_socket, SensorSocket *socket_b) : Mode(requirements, light_source_socket, socket_b)
  {
    result = new ResultExposure(1);

    selector = new ItemsSelector<long>(SelectionItems::EV, SIZEOF(SelectionItems::EV), 0, 15, 0);
    selector->set_format("EV %2s");
  };

  void start()
  {
    running = true;
    Encoder.stop();
    light_source->fanOn();
    Counter.start();
  }

  void stop()
  {
    running = false;
    Counter.stop();
    light_source->off();
    Encoder.start();
  }

  void run()
  {
    int selected = -1;

    do
    {
      // Exposure staff
      if (Counter.available())
      {
        // skip fantom counts
        if (Counter.counts > 2)
        {
          result->print();
          Buzzer.beep();
        }
        Counter.reset();
      }

      // EV settings
      Encoder.update();

      int enc_value = Encoder.getValue();

      if (enc_value < 0)
      {
        selector->next();
      }

      if (enc_value > 0)
      {
        selector->prev();
      }

      if (selected != selector->selected)
      {
        selected = selector->selected;

        light_source->setEV(Settings::KValue.value, (selected + Settings::EVShift.value));
      }

      // Stop condition
      if (StopButton.pressed())
      {
        stop();
      }

    } while (running);
  }

  void calibrate()
  {
    Encoder.stop();
    Buzzer.beep();
    light_source->fanOn();

    result->print_calibration();

    // Calibrate at K12
    Settings::K12Exposure.update(calibration_value(0, EV12_INDEX));

    // Calibrate at K14
    Settings::K14Exposure.update(calibration_value(1, EV12_INDEX));

    // Calibrate at K12+1
    Settings::K12ShiftedExposure.update(calibration_value(0, EV13_INDEX));

    // Calibrate at K14+1
    Settings::K14ShiftedExposure.update(calibration_value(1, EV13_INDEX));


    light_source->off();
    Buzzer.bebeep();
    Encoder.start();
  }

  unsigned long calibration_value(byte k, byte ev)
  {
    light_source->setEV(k, ev);
    delay(1000);
    return Counter.calibrate();
  }

};

#endif
