/*

    Mode Focal Base

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _MODE_FLASH_FOCAL_H
#define _MODE_FLASH_FOCAL_H

class ModeFlashFocal : public Mode
{

public:
  template <byte N>
  ModeFlashFocal(Requirement (&requirements)[N], LightUnitSocket *light_source_socket, SensorSocket *socket_a, SensorSocket *socket_b) : Mode(requirements, light_source_socket, socket_a, socket_b)
  {
    result = new ResultFlashSync(1);
  };

  void take_measurement()
  {

    // A - top right sensor
    byte sensor_a_mask = sockets[0]->PinToBitMask(0);
    // B - center censor
    byte sensor_b_mask = sockets[0]->PinToBitMask(1);
    // C - bottom left sensor
    byte sensor_c_mask = sockets[0]->PinToBitMask(2);
    // Flash sync sensor on Socket B, but on the same port
    byte flash_sensor_mask = sockets[1]->PinToBitMask(0);

    byte sensor_port = sockets[0]->PinToPort(1);


    byte fired_flash = 1;
    byte fired_a = 2;
    byte fired_b = 2;
    byte fired_c = 2;
    byte measure = 4;

    Timer2::Count flash;
    Timer2::Count start_a;
    Timer2::Count stop_a;
    Timer2::Count start_b;
    Timer2::Count stop_b;
    Timer2::Count start_c;
    Timer2::Count stop_c;

    do
    {
      //  read Flash Sync Sensor
      if (!(*portInputRegister(sensor_port) & flash_sensor_mask) && (fired_flash == 1))
      {
        Timer.get_count(&flash);
        fired_flash--;
        measure--;
      }

      //  read Sensor A
      if (!(*portInputRegister(sensor_port) & sensor_a_mask) && (fired_a == 2))
      {
        Timer.get_count(&start_a);
        fired_a--;
      }
      if ((*portInputRegister(sensor_port) & sensor_a_mask) && (fired_a == 1))
      {
        Timer.get_count(&stop_a);
        fired_a--;
        measure--;
      }

      //  read Sensor B
      if (!(*portInputRegister(sensor_port) & sensor_b_mask) && (fired_b == 2))
      {
        Timer.get_count(&start_b);
        fired_b--;
      }
      if ((*portInputRegister(sensor_port) & sensor_b_mask) && (fired_b == 1))
      {
        Timer.get_count(&stop_b);
        fired_b--;
        measure--;
      }

      //  read Sensor C
      if (!(*portInputRegister(sensor_port) & sensor_c_mask) && (fired_c == 2))
      {
        Timer.get_count(&start_c);
        fired_c--;
      }
      if ((*portInputRegister(sensor_port) & sensor_c_mask) && (fired_c == 1))
      {
        Timer.get_count(&stop_c);
        fired_c--;
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
      if (start_a.total_count() < start_c.total_count())
      {
        // A->C (Up-to-Down shutter or Right-to-Left)
        result->add_measurement(start_b.total_count(), stop_b.total_count());
        // +X
        result->add_measurement(start_c.total_count(), flash.total_count());
        // X-D
        result->add_measurement(flash.total_count(), stop_a.total_count());
        // -X (FP)
        result->add_measurement(flash.total_count(), start_a.total_count());
      }
      else
      {
        // C->A (Down-to-Up shutter OR Sensor is upside down (Left-to-Right doesn't exists))
        result->add_measurement(start_b.total_count(), stop_b.total_count());
        // +X
        result->add_measurement(start_a.total_count(), flash.total_count());
        // X-D
        result->add_measurement(flash.total_count(), stop_c.total_count());
        // -X (FP)
        result->add_measurement(flash.total_count(), start_c.total_count());
      }
      result->add_test();
    }
  }
};

#endif
