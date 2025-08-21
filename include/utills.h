/*
    Film Camera Tester
    Copyright (C) 2021  Serhiy Rozum
*/

#pragma once

#include <Arduino.h>
#include "config.h"

// Macros

#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))

#define SIZEOF(a) sizeof(a) / sizeof(*a)

namespace Utills
{

  void ShowWelcomeScreen()
  {
    LCD.setCursor(1, 1);
    LCD.print(F("SRT  Camera Tester"));
    LCD.setCursor(4, 2);
    LCD.print(F("Version "));
    LCD.print(SRT_VERSION_MAJOR);
    LCD.print(".");
    LCD.print(SRT_VERSION_MINOR);
  }

  // Helper functions

  template <typename V, typename T>
  T find_item(V value, const T *t, byte size)
  {
    T item;
    for (int i = (size - 1); i >= 0; --i)
    {
      memcpy_P(&item, &t[i], sizeof(T));
      if (item.value == value)
        return item;
    }
    return item; // t[0] returned by default
  }

  template <typename V, typename T, byte N>
  T find_item(V value, const T (&t)[N])
  {
    T item;
    for (int i = (N - 1); i >= 0; --i)
    {
      memcpy_P(&item, &t[i], sizeof(T));
      if (item.value == value)
        return item;
    }
    return item; // t[0] returned by default
  }

  template <typename V, typename T>
  int find_index_of(V value, const T *t, byte size)
  {
    T item;
    for (int i = (size - 1); i >= 0; --i)
    {
      memcpy_P(&item, &t[i], sizeof(T));
      if (item.value == value)
        return i;
    }
    return 0;
  }

  template <typename V, typename T, byte N>
  int find_index_of(V value, const T (&t)[N])
  {
    T item;
    for (int i = (N - 1); i >= 0; --i)
    {
      memcpy_P(&item, &t[i], sizeof(T));
      if (item.value == value)
        return i;
    }
    return 0;
  }

}

