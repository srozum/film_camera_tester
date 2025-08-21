/*

    Mode Focal
    Same Mode used for Vertical and Horizontal curtain travel shutters.
    Direction of a curtains detected automatically between Left/Right or Up/Down directions

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _MODE_FOCAL_H
#define _MODE_FOCAL_H

class ModeFocal : public Mode
{

public:
  bool vertical = true;

  template <byte N>
  ModeFocal(Requirement (&requirements)[N], LightUnitSocket *light_source_socket, SensorSocket *socket_a, bool vertical = true) : Mode(requirements, light_source_socket, socket_a), vertical(vertical)
  {
    result = new ResultFocal(1);
    result->frame_space = (vertical) ? Settings::FrameHeight.value : Settings::FrameWidth.value;

    selector = new ItemsSelector<long>(SelectionItems::ResultFocalFormat, SIZEOF(SelectionItems::ResultFocalFormat), 0, 15, 0);
    selector->set_format("%-5s");
  };

  void start()
  {
    Mode::start();

    result->num_of_tests = (result->selected_item.value == 3) ? Settings::Series.value : 1;
    result->sensor_space = (vertical) ? ((sockets[0]->device_id() == DeviceID::SENSOR_FP1) ? 22 : 20) : 32;
  }

  void take_measurement()
  {

    // A - top right sensor
    byte sensor_a_mask = sockets[0]->PinToBitMask(0);
    // B - center censor
    byte sensor_b_mask = sockets[0]->PinToBitMask(1);
    // C - bottom left sensor
    byte sensor_c_mask = sockets[0]->PinToBitMask(2);

    byte sensor_port = sockets[0]->PinToPort(1);

    Timer2::Count start_a;
    Timer2::Count stop_a;
    Timer2::Count start_b;
    Timer2::Count stop_b;
    Timer2::Count start_c;
    Timer2::Count stop_c;

    byte fired_a = 2;
    byte fired_b = 2;
    byte fired_c = 2;
    byte measure = 3;

    do
    {
      // Read A->B->C

      //  read openings
      if (!(*portInputRegister(sensor_port) & sensor_a_mask) && (fired_a == 2))
      {
        Timer.get_count(&start_a);
        fired_a--;
      }
      if (!(*portInputRegister(sensor_port) & sensor_b_mask) && (fired_b == 2))
      {
        Timer.get_count(&start_b);
        fired_b--;
      }
      if (!(*portInputRegister(sensor_port) & sensor_c_mask) && (fired_c == 2))
      {
        Timer.get_count(&start_c);
        fired_c--;
      }

      // read closings
      if ((*portInputRegister(sensor_port) & sensor_a_mask) && (fired_a == 1))
      {
        Timer.get_count(&stop_a);
        fired_a--;
        measure--;
      }
      if ((*portInputRegister(sensor_port) & sensor_b_mask) && (fired_b == 1))
      {
        Timer.get_count(&stop_b);
        fired_b--;
        measure--;
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
        // Up-to-Down shutter or Right-to-Left
        result->add_measurement(start_a.total_count(), stop_a.total_count());
        result->add_measurement(start_b.total_count(), stop_b.total_count());
        result->add_measurement(start_c.total_count(), stop_c.total_count());
        // Curtain Travel Times
        result->add_measurement(start_a.total_count(), start_c.total_count());
        result->add_measurement(stop_a.total_count(), stop_c.total_count());
      }
      else
      {
        // Down-to-Up shutter OR Sensor is upside down (Left-to-Right doesn't exists)
        result->add_measurement(start_c.total_count(), stop_c.total_count());
        result->add_measurement(start_b.total_count(), stop_b.total_count());
        result->add_measurement(start_a.total_count(), stop_a.total_count());
        // Curtain Travel Times
        result->add_measurement(start_c.total_count(), start_a.total_count());
        result->add_measurement(stop_c.total_count(), stop_a.total_count());
      }
      result->add_test();
    }
  }
};

#endif
