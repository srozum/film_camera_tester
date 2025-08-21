/*

    Mode Simple Continuous

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _MODE_SIMPLE_H
#define _MODE_SIMPLE_H

class ModeSimple : public Mode
{

public:
  template <byte N>
  ModeSimple(Requirement (&requirements)[N], LightUnitSocket *light_source_socket, SensorSocket *socket_a, byte num_of_tests) : Mode(requirements, light_source_socket, socket_a)
  {
    result = new ResultSimple(num_of_tests);

    set_speeds_selector();
  };

  void take_measurement()
  {

    byte sensor_mask = sockets[0]->PinToBitMask(1);
    byte sensor_port = sockets[0]->PinToPort(1);

    Timer2::Count opened;
    Timer2::Count closed;

    byte measure = 2;

    do
    {
      if (!(*portInputRegister(sensor_port) & sensor_mask) && (measure == 2))
      {
        Timer.get_count(&opened);
        measure--;
      }
      if ((*portInputRegister(sensor_port) & sensor_mask) && (measure == 1))
      {
        Timer.get_count(&closed);
        measure--;
      }

      if (StopButton.pressed() == true)
      {
        measure = 0;
        stop();
      }
    } while (measure);

    if ((measure == 0) && (running == true))
    {
      result->add_measurement(opened.total_count(), closed.total_count());
      result->add_test();
    }
  }
};

#endif
