/*

    Mode Light Source

    Copyright (C) 2021  Serhiy Rozum

*/
#pragma once

#ifndef _MODE_LIGHT_SOURCE_H
#define _MODE_LIGHT_SOURCE_H

class ModeLightSource : public Mode
{

public:
  template <byte N>
  ModeLightSource(Requirement (&requirements)[N], LightUnitSocket *light_source_socket) : Mode(requirements, light_source_socket)
  {
    result = new ResultLightSource(0);

    selector = new ItemsSelector<long>(SelectionItems::EV, SIZEOF(SelectionItems::EV), 0, 15, 0);
    selector->set_format("EV %2s");
  };

  void start()
  {
    running = true;
    light_source->fanOn();
  }

  void stop()
  {
    light_source->off();
    running = false;
  }

  void run()
  {

    int selected = -1;

    do
    {

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

        result->selected_item = selector->selected_item();
        result->print();

        light_source->setEV(Settings::KValue.value, (selected + Settings::EVShift.value));

        delay(ENCODER_DELAY);
      }

      if (StopButton.pressed() == true)
      {
        stop();
      }

    } while (running);
  }

};

#endif
