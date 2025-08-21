/*
    Dictionary of Selectable items
    Copyright (C) 2021  Serhiy Rozum
*/

#pragma once

#ifndef _SELECTION_ITEMS_H
#define _SELECTION_ITEMS_H

#include <Arduino.h>

template <typename T>
struct SelectionItem
{
    T value;
    const char *label;
};

typedef SelectionItem<byte> ByteSelectionItem;
typedef SelectionItem<long> LongSelectionItem;

namespace SelectionItems
{

    // Settings Selection Items

    const ByteSelectionItem OnOff[] PROGMEM = {
        {0, "Off"},
        {1, "On"},
    };

    const ByteSelectionItem FrameWidth[] PROGMEM = {
        {30, "30mm"},
        {32, "32mm"},
        {34, "34mm"},
        {36, "36mm"},
    };

    const ByteSelectionItem FrameHeight[] PROGMEM = {
        {20, "20mm"},
        {22, "22mm"},
        {24, "24mm"},
    };

    const ByteSelectionItem SeriesSize[] PROGMEM = {
        {3, "3"},
        {5, "5"},
        {10, "10"},
        {20, "20"},
    };

    const ByteSelectionItem SpeedsRange[] PROGMEM = {
        {0, "Old"},
        {1, "New"},
        {2, "Log"},
    };

    const ByteSelectionItem KValue[] PROGMEM = {
        {0, "12.50"},
        {1, "14.03"},
    };

    const ByteSelectionItem EVShift[] PROGMEM = {
        {0, "0EV"},
        {1, "+1EV"},
    };

    // Modes Selection Items

    const LongSelectionItem ResultFocalFormat[] PROGMEM = {
        {0, "Time"},
        {1, "Exp."},
        {2, "Curt."},
        {3, "Avg."},
    };

    // 1s, 1/2, 1/5, 1/10, 1/25, 1/50, 1/100, 1/200, 1/250, 1/300, 1/500, 1/1000
    const LongSelectionItem SpeedsListOld[] PROGMEM = {
        {0, "Auto"},
        {8000000, "8s"},
        {4000000, "4s"},
        {2000000, "2s"},
        {1000000, "1s"},
        {500000, "\x7"
                 "2"},
        {200000, "\x7"
                 "5"},
        {100000, "\x7"
                 "10"},
        {40000, "\x7"
                "25"},
        {20000, "\x7"
                "50"},
        {10000, "\x7"
                "100"},
        {5000, "\x7"
               "200"},
        {4000, "\x7"
               "250"},
        {3333, "\x7"
               "300"},
        {2000, "\x7"
               "500"},
        {1000, "\x7"
               "1000"},
    };

    // 1s..1/8000
    const LongSelectionItem SpeedsListNew[] PROGMEM = {
        {0, "Auto"},
        {8000000, "8s"},
        {4000000, "4s"},
        {2000000, "2s"},
        {1000000, "1s"},
        {500000, "\x7"
                 "2"},
        {250000, "\x7"
                 "4"},
        {125000, "\x7"
                 "8"},
        {66666, "\x7"
                "15"},
        {33333, "\x7"
                "30"},
        {16666, "\x7"
                "60"},
        {8000, "\x7"
               "125"},
        {4000, "\x7"
               "250"},
        {2000, "\x7"
               "500"},
        {1000, "\x7"
               "1000"},
        {500, "\x7"
              "2000"},
        {250, "\x7"
              "4000"},
        {125, "\x7"
              "8000"},
    };

    // Logarithmic 2^2
    const LongSelectionItem SpeedsListLog[] PROGMEM = {
        {0, "Auto"},
        {8000000, "8s"},
        {4000000, "4s"},
        {2000000, "2s"},
        {1000000, "1s"},
        {500000, "\x7"
                 "2"},
        {250000, "\x7"
                 "4"},
        {125000, "\x7"
                 "8"},
        {62500, "\x7"
                "16"},
        {31250, "\x7"
                "32"},
        {15625, "\x7"
                "64"},
        {7812, "\x7"
               "128"},
        {3937, "\x7"
               "254"},
        {1952, "\x7"
               "512"},
        {976, "\x7"
              "1024"},
        {488, "\x7"
              "2048"},
        {244, "\x7"
              "4096"},
        {122, "\x7"
              "8192"},
    };

    const LongSelectionItem EV[] PROGMEM = {
        {5, "5"},
        {6, "6"},
        {7, "7"},
        {8, "8"},
        {9, "9"},
        {10, "10"},
        {11, "11"},
        {12, "12"},
        {13, "13"},
        {14, "14"},
        {15, "15"},
        {16, "16"},
    };

}
#endif
