/*

    Modes Parent Class

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _MODE_H
#define _MODE_H

#include <Arduino.h>

#define MODE_MAX_SOCKETS 2

class Mode
{

public:
  struct Requirement
  {
    SocketID socket_id;
    DeviceID device_id;
  };

  Requirement *requirements = nullptr;
  byte num_of_requirements;

  LightUnitSocket *light_source = nullptr;

  SensorSocket *sockets[MODE_MAX_SOCKETS];
  int num_of_sockets = 0;

  Result *result = nullptr;

  ItemsSelector<long> *selector = nullptr;

  boolean running = false;

  Mode(){};

  template <byte N>
  Mode(Requirement (&requirements)[N]) : requirements(requirements), num_of_requirements(N){};

  template <byte N>
  Mode(Requirement (&requirements)[N], LightUnitSocket *light_source_socket) : Mode(requirements)
  {
    light_source = light_source_socket;
  };

  template <byte N>
  Mode(Requirement (&requirements)[N], LightUnitSocket *light_source_socket, SensorSocket *socket_a) : Mode(requirements, light_source_socket)
  {
    register_socket(socket_a);
  };

  template <byte N>
  Mode(Requirement (&requirements)[N], LightUnitSocket *light_source_socket, SensorSocket *socket_a, SensorSocket *socket_b) : Mode(requirements, light_source_socket, socket_a)
  {
    register_socket(socket_b);
  };

  virtual ~Mode()
  {
    if (result != nullptr)
    {
      result->clear_measurements();
      result->clear_stats();
      delete result;
      result = NULL;
    }
    if (selector != nullptr)
    {
      delete selector;
      selector = NULL;
    }
  }

  void set_speeds_selector()
  {
    if (Settings::Speeds.value == 0)
    {
      selector = new ItemsSelector<long>(SelectionItems::SpeedsListOld, SIZEOF(SelectionItems::SpeedsListOld), 0, 15, 0);
    }
    else if (Settings::Speeds.value == 1)
    {
      selector = new ItemsSelector<long>(SelectionItems::SpeedsListNew, SIZEOF(SelectionItems::SpeedsListNew), 0, 15, 0);
    }
    else
    {
      selector = new ItemsSelector<long>(SelectionItems::SpeedsListLog, SIZEOF(SelectionItems::SpeedsListLog), 0, 15, 0);
    }
    selector->set_format("%-5s");
  }

  // Measurement methods

  bool is_ready()
  {
    if (num_of_requirements == 0)
      return true;

    if (num_of_sockets > 0)
    {
      for (uint8_t s = 0; s < num_of_sockets; s++)
      {
        if (!sockets[s]->connected())
          return false;

        bool ready = false;

        for (uint8_t r = 0; r < num_of_requirements; r++)
        {
          if ((requirements[r].socket_id == sockets[s]->id) && (sockets[s]->device_id() == requirements[r].device_id))
          {
            ready = true;
            break;
          }
        }

        if (!ready)
          return false;
      }
    }

    bool ready = true;

    for (uint8_t r = 0; r < num_of_requirements; r++)
    {
      if (requirements[r].socket_id == light_source->id)
      {
        ready = false;

        if (requirements[r].device_id == light_source->device_id())
        {
          ready = true;
          break;
        }
      }
    }

    return ready;
  }

  virtual void start()
  {
    Encoder.stop();

    if (result && selector)
    {
      result->selected_item = selector->selected_item();
    }

    if (sockets[0]->device_id() == DeviceID::SENSOR_LASER)
    {
      sockets[0]->setPin(2, HIGH);
    }
    else if (light_source->connected())
    {
      light_source->on();
    }

    Timer.start();

    running = true;
  }

  virtual void run()
  {
    if (result)
    {
      if (result->finished())
      {
        result->reset();
      }
      else
      {
        result->clear_measurements();
      }
    }

    Timer.reset();

    take_measurement();

    if (result && (running == true))
    {
      if ((result->num_of_tests == 1) || !result->finished())
      {
        Buzzer.beep();
      }
      else
      {
        Buzzer.bebeep();
      }
    }
  }

  virtual void stop()
  {
    running = false;

    Timer.stop();

    // Light Off
    if (sockets[0]->device_id() == DeviceID::SENSOR_LASER)
    {
      sockets[0]->setPin(2, LOW);
    }
    else if (light_source->connected())
    {
      light_source->off();
    }

    // always clear at stop to prevent memory leaks
    if (result)
    {
      result->reset();
    }

    Encoder.start();
  }

  // Main method to overload.
  virtual void take_measurement() {}

  virtual void calibrate() {}

  virtual void print_label()
  {
    if (selector)
      selector->show();
  };

  virtual void selector_next()
  {
    if (selector)
    {
      selector->next();
    }
  }

  virtual void selector_prev()
  {
    if (selector)
    {
      selector->prev();
    }
  }

  void register_socket(SensorSocket *socket)
  {
    sockets[num_of_sockets] = socket;
    num_of_sockets++;
  }
};

#endif
