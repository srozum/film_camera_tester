/*

    Settings Menu

    Copyright (C) 2021  Serhiy Rozum

*/

#pragma once

#ifndef _SETTINGS_MENU_H
#define _SETTINGS_MENU_H

#include <Arduino.h>
#include "config.h"
#include "utills.h"
#include "glyphs.h"
#include "settings.h"
#include "selection_items.h"
#include "items_selector.h"

typedef struct
{
  const __FlashStringHelper *label;
  Setting<byte> *setting;
  const ByteSelectionItem *selections;
  uint8_t num_of_selections;
} SettingsMenuItem;

// 13 chars
const char MenuKValueLabel[] PROGMEM = "K Value";
const char MenuEVShiftLabel[] PROGMEM = "EV Shift";
const char MenuSpeedsRangeLabel[] PROGMEM = "Speeds Range";
const char MenuTestSeriesLabel[] PROGMEM = "Test Series";
const char MenuFrameWidthLabel[] PROGMEM = "Frame Width";
const char MenuFrameHeightLabel[] PROGMEM = "Frame Height";
const char MenuSoundLabel[] PROGMEM = "Sound";
const char MenuExitMenuLabel[] PROGMEM = "Exit Menu";

SettingsMenuItem settings[] = {
    {FPSTR(MenuKValueLabel), &Settings::KValue, SelectionItems::KValue, SIZEOF(SelectionItems::KValue)},
    {FPSTR(MenuEVShiftLabel), &Settings::EVShift, SelectionItems::EVShift, SIZEOF(SelectionItems::EVShift)},
    {FPSTR(MenuFrameWidthLabel), &Settings::FrameWidth, SelectionItems::FrameWidth, SIZEOF(SelectionItems::FrameWidth)},
    {FPSTR(MenuFrameHeightLabel), &Settings::FrameHeight, SelectionItems::FrameHeight, SIZEOF(SelectionItems::FrameHeight)},
    {FPSTR(MenuSpeedsRangeLabel), &Settings::Speeds, SelectionItems::SpeedsRange, SIZEOF(SelectionItems::SpeedsRange)},
    {FPSTR(MenuTestSeriesLabel), &Settings::Series, SelectionItems::SeriesSize, SIZEOF(SelectionItems::SeriesSize)},
    {FPSTR(MenuSoundLabel), &Settings::Sound, SelectionItems::OnOff, SIZEOF(SelectionItems::OnOff)},
    {FPSTR(MenuExitMenuLabel), nullptr},
};

class SettingsMenu
{

public:
  template <byte N>
  SettingsMenu(SettingsMenuItem (&items)[N]) : items(items), num_of_items(N){};

  void select()
  {

    selected = 0;

    int enc_last = -1;
    int enc_value = selected;
    int window_pos = 0;

    do
    {

      enc_value += Encoder.getValue();

      if (enc_value < 0)
      {
        enc_value = 0;
      }
      if (enc_value > (num_of_items - 1))
      {
        enc_value = num_of_items - 1;
      }

      if (enc_value != enc_last)
      {
        enc_last = enc_value;
        if (enc_value > (window_pos + (MENU_SIZE - 1)))
        {
          window_pos = enc_value - (MENU_SIZE - 1);
        }
        if (enc_value < window_pos)
        {
          window_pos = enc_value;
        }
        for (byte i = 0; i < MENU_SIZE; i++)
        {

          LCD.setCursor(0, i);
          if (enc_value == (window_pos + i))
          {
            selected_pos = i;
            LCD.write((uint8_t)Glyph::ARR_RIGHT);
          }
          else
          {
            LCD.write((uint8_t)Glyph::NONE);
          }

          const char *selected_item_label;

          // print label of selected item
          if (items[window_pos + i].setting != nullptr)
          {
            ByteSelectionItem selected_item;

            for (byte j = 0; j < items[window_pos + i].num_of_selections; j++)
            {

              memcpy_P(&selected_item, &items[window_pos + i].selections[j], sizeof(ByteSelectionItem));

              if (items[window_pos + i].setting->value == selected_item.value)
              {
                selected_item_label = selected_item.label;
                break;
              }
            }
          }
          else
          {
            selected_item_label = "";
          }

          char buf[19];
          sprintf_P(buf, PSTR("%-13S %5s"), items[window_pos + i].label, selected_item_label);
          LCD.print(buf);
        }

        delay(ENCODER_DELAY);
      }

      if (EncoderButton.pressed())
      {
        if (items[enc_value].setting == nullptr)
        {
          return;
        }
        else
        {
          selected = enc_value;
          select_setting_value();
        }
      }

      if (StopButton.pressed())
      {
        return;
      }

    } while (true);
  }

private:
  int selected = 0;
  byte selected_pos;

  SettingsMenuItem *items;
  uint8_t num_of_items;

  ItemsSelector<byte> *selector;

  void select_setting_value()
  {

    selector = new ItemsSelector<byte>(items[selected].selections, items[selected].num_of_selections, items[selected].setting->value, 15, selected_pos, true);

    selector->select();

    items[selected].setting->update(selector->selected_item().value);

    delete selector;
    selector = NULL;
  }
};

SettingsMenu Menu(settings);

#endif
