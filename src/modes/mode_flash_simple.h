/*

    Mode Simple Flash

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _MODE_FLASH_SIMPLE_H
#define _MODE_FLASH_SIMPLE_H

class ModeFlashSimple : public Mode
{

public:
  template <byte N>
  ModeFlashSimple(Requirement (&requirements)[N], LightUnitSocket *light_source_socket, SensorSocket *socket_a, SensorSocket *socket_b) : Mode(requirements, light_source_socket, socket_a, socket_b)
  {
    result = new ResultFlashSync(1);
  };

  void take_measurement()
  {

    byte sensor_mask = sockets[0]->PinToBitMask(1);
    byte flash_sensor_mask = sockets[1]->PinToBitMask(0);

    byte sensor_port = sockets[0]->PinToPort(1);

    Timer2::Count opened;
    Timer2::Count closed;
    Timer2::Count flash;

    // X and M both around shutter OPENING, not closing!!

    byte flash_fired = 1;
    byte shutter_fired = 2;
    byte measure = 3;

    do
    {
      if (!(*portInputRegister(sensor_port) & flash_sensor_mask) && (flash_fired == 1))
      {
        Timer.get_count(&flash);
        flash_fired--;
        measure--;
      }

      if (!(*portInputRegister(sensor_port) & sensor_mask) && (shutter_fired == 2))
      {
        Timer.get_count(&opened);
        shutter_fired--;
        measure--;
      }
      if ((*portInputRegister(sensor_port) & sensor_mask) && (shutter_fired == 1))
      {
        Timer.get_count(&closed);
        shutter_fired--;
        measure--;
      }

      if (StopButton.pressed() == true)
      {
        measure = 0;
        stop();
      }
    } while (measure > 0);

    if ((measure == 0) && (running == true))
    {
      result->add_measurement(opened.total_count(), closed.total_count());
      result->add_measurement(flash.total_count(), opened.total_count());
      result->add_test();
    }
  }
};

#endif
