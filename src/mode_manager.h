/*

    Mode Manager

    Copyright (C) 2021  Serhiy Rozum

*/

#pragma once

#ifndef _MODE_MANAGER_H
#define _MODE_MANAGER_H

#include <Arduino.h>
#include "config.h"

enum class ModeID : uint8_t
{
  Simple,
  FocalVertical,
  FocalHorizontal,
  SeriesSimple,
  FlashSimple,
  FlashFocal,
  LeafEfficiency,
  LightSource,
  Exposure,
};

typedef struct
{
  ModeID id;
  const __FlashStringHelper *name;
  const __FlashStringHelper *short_name;
} ModeItem;

ModeItem ModesList[] = {
    {ModeID::Simple, FPSTR(ModeSimpleName), FPSTR(ShortNameSimple)},
    {ModeID::FocalVertical, FPSTR(ModeFocalVerticalName), FPSTR(ShortNameFocalVertical)},
    {ModeID::FocalHorizontal, FPSTR(ModeFocalHorizontalName), FPSTR(ShortNameFocalHorizontal)},
    {ModeID::LightSource, FPSTR(ModeLightSourceName), FPSTR(ShortNameLight)},
    {ModeID::Exposure, FPSTR(ModeExposureName), FPSTR(ShortExposure)},
    {ModeID::SeriesSimple, FPSTR(ModeSeriesSimpleName), FPSTR(ShortNameSeries)},
    {ModeID::FlashSimple, FPSTR(ModeFlashSimpleName), FPSTR(ShortNameFlashSimple)},
    {ModeID::FlashFocal, FPSTR(ModeFlashFocalName), FPSTR(ShortNameFlashFocal)},
    {ModeID::LeafEfficiency, FPSTR(ModeLeafEfficiencyName), FPSTR(ShortNameLeafEff)},
};

Mode::Requirement SimpleRequirements[] = {
    {SocketID::A, DeviceID::SENSOR_DOT},
    {SocketID::A, DeviceID::SENSOR_LASER},
    {SocketID::A, DeviceID::SENSOR_FP1},
    {SocketID::A, DeviceID::SENSOR_FP2},
};

Mode::Requirement FocalRequirements[] = {
    {SocketID::A, DeviceID::SENSOR_FP1},
    {SocketID::A, DeviceID::SENSOR_FP2},
};

Mode::Requirement FlashSimpleRequirements[] = {
    {SocketID::A, DeviceID::SENSOR_DOT},
    {SocketID::A, DeviceID::SENSOR_LASER},
    {SocketID::A, DeviceID::SENSOR_FP1},
    {SocketID::A, DeviceID::SENSOR_FP2},
    {SocketID::B, DeviceID::SENSOR_FLASH},
};

Mode::Requirement FlashFocalRequirements[] = {
    {SocketID::A, DeviceID::SENSOR_FP1},
    {SocketID::A, DeviceID::SENSOR_FP2},
    {SocketID::B, DeviceID::SENSOR_FLASH},
};

Mode::Requirement LaserSensorRequirements[] = {
    {SocketID::A, DeviceID::SENSOR_LASER},
};

Mode::Requirement LightSourceRequirements[] = {
    {SocketID::LIGHT_UNIT, DeviceID::LIGHT_UNIT_2},
};

Mode::Requirement ExposureRequirements[] = {
    {SocketID::B, DeviceID::SENSOR_EXP},
    {SocketID::LIGHT_UNIT, DeviceID::LIGHT_UNIT_2},
};

class ModeManager
{

public:
  Mode *mode = nullptr;
  ModeID mode_id;

  template <byte N>
  ModeManager(ModeItem (&modes)[N]) : modes(modes), num_of_modes(N){};

  void init()
  {
    selected = Settings::Mode.value;
    set_mode(modes[selected].id);
  }

  void select()
  {
    int enc_last = -1;
    int enc_value = selected;
    int max_value = num_of_modes - 1;
    int window_size = MENU_SIZE - 1;
    int window_pos = min(enc_value, (max_value - window_size)); // bug if less than 3 elements

    do
    {
      enc_value += Encoder.getValue();

      if (enc_value < 0)
      {
        enc_value = 0;
      }
      if (enc_value > max_value)
      {
        enc_value = max_value;
      }

      if (enc_value != enc_last)
      {
        enc_last = enc_value;

        if (enc_value > (window_pos + window_size))
        {
          window_pos = enc_value - window_size;
        }
        if (enc_value < window_pos)
        {
          window_pos = enc_value;
        }
        for (int i = 0; i < MENU_SIZE; i++)
        {

          LCD.setCursor(0, i);
          if (enc_value == (window_pos + i))
          {
            LCD.write((uint8_t)Glyph::ARR_RIGHT);
          }
          else
          {
            LCD.write((uint8_t)Glyph::NONE);
          }

          char buf[19];
          sprintf_P(buf, PSTR("%-19S"), modes[window_pos + i].name);
          LCD.print(buf);
        }

        delay(ENCODER_DELAY);
      }

      if (EncoderButton.pressed())
      {
        if (enc_value != selected)
        {
          selected = enc_value;

          Settings::Mode.update(selected);

          set_mode(modes[selected].id);
        }
        return;
      }

      if (StopButton.pressed())
      {
        return;
      }

      delay(ENCODER_DELAY);

    } while (true);
  }

  void set_mode(ModeID m)
  {
    mode_id = m;

    if (mode != nullptr)
    {
      delete mode;
      mode = NULL;
    }

    switch (m)
    {
    case ModeID::Simple:
      mode = new ModeSimple(SimpleRequirements, &LightSource, &SocketA, 1);
      break;
    case ModeID::FocalVertical:
      mode = new ModeFocal(FocalRequirements, &LightSource, &SocketA, true);
      break;
    case ModeID::FocalHorizontal:
      mode = new ModeFocal(FocalRequirements, &LightSource, &SocketA, false);
      break;
    case ModeID::SeriesSimple:
      mode = new ModeSimple(SimpleRequirements, &LightSource, &SocketA, Settings::Series.value);
      break;
    case ModeID::FlashSimple:
      mode = new ModeFlashSimple(FlashSimpleRequirements, &LightSource, &SocketA, &SocketB);
      break;
    case ModeID::FlashFocal:
      mode = new ModeFlashFocal(FlashFocalRequirements, &LightSource, &SocketA, &SocketB);
      break;
    case ModeID::LeafEfficiency:
      mode = new ModeLeafEfficiency(LaserSensorRequirements, &LightSource, &SocketA);
      break;
    case ModeID::LightSource:
      mode = new ModeLightSource(LightSourceRequirements, &LightSource);
      break;
    case ModeID::Exposure:
      mode = new ModeExposure(ExposureRequirements, &LightSource, &SocketB);
      break;
    }
  }

  bool is_ready()
  {
    return mode->is_ready();
  }

  bool is_runing()
  {
    return mode->running;
  }

  void reload()
  {
    stop();
    set_mode(mode_id);
  }

  void update()
  {
    // Status Bar
    char buff[20];
    sprintf_P(buff, PSTR("[%c|%c] %-8S"), (uint8_t)SocketA.sensor_glyph(), (uint8_t)SocketB.sensor_glyph(), modes[selected].short_name);
    LCD.setCursor(0, 0);
    LCD.print(buff);

    mode->print_label();

    // Info Area

    if (mode->running)
    {
      if (mode->result != nullptr)
      {
        mode->result->print();
      }
    }
    else if (mode->is_ready())
    {
      LCD.setCursor(4, 2);
      LCD.print(F("Press  Start"));
    }
    else
    {
      LCD.setCursor(4, 2);
      LCD.print(F(" Not  Ready "));
    }
  }

  void start()
  {
    if (mode->is_ready())
    {
      Buzzer.beep();
      mode->start();
    }
    else
    {
      Buzzer.error();
    }
  }

  void run()
  {
    mode->run();
  }

  // Emergency stop in case if Sensor was disconnected
  void stop()
  {
    if (mode->running)
    {
      mode->stop();
      Buzzer.error();
    }
  }

  void calibrate()
  {
    if (mode->is_ready())
    {
      mode->calibrate();
    }
    else
    {
      Buzzer.error();
    }
  }

  void selector_next()
  {
    mode->selector_next();
  }

  void selector_prev()
  {
    mode->selector_prev();
  }

private:
  ModeItem *modes;
  const uint8_t num_of_modes;
  int selected = 0;
};

ModeManager ModeMgr(ModesList);

#endif
