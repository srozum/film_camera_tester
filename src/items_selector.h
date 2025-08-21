/*

    Items Selector

    Displays SettingItems and allows to cycle through them either using Encoder directly or by
    driving selection from outside using methods 'next' / 'prev'

    Copyright (C) 2021  Serhiy Rozum

*/

#pragma once

#ifndef _SETTINGS_SELECTOR_H
#define _SETTINGS_SELECTOR_H

#include <Arduino.h>
#include "config.h"
#include "utills.h"
#include "selection_items.h"

template <typename T> 
class ItemsSelector
{

public:
    byte selected;

    ItemsSelector(const SelectionItem<T> *items, uint8_t num_of_items, T selected_value, uint8_t col, uint8_t row, bool blink = false)
        : items(items), num_of_items(num_of_items), _col(col), _row(row), _blink(blink)
    {
        selected = Utills::find_index_of(selected_value, items, num_of_items);
    };

    // Returns index of selected value
    void select()
    {

        int enc_value;

        show();

        delay(200);

        do
        {
            enc_value = Encoder.getValue();

            if (enc_value < 0)
            {
                next();
            }
            if (enc_value > 0)
            {
                prev();
            }

            if (EncoderButton.pressed())
            {
                if (_blink)
                {
                    LCD.noBlink();
                }
                Buzzer.beep();

                delay(ENCODER_DELAY);

                return;
            }

        } while (true);
    }

    void next()
    {
        if (selected < (num_of_items - 1))
        {
            selected++;
            show();
        }
    }

    void prev()
    {
        if (selected > 0)
        {
            selected--;
            show();
        }
    }

    void show()
    {
        LCD.setCursor(_col, _row);

        char buf[7];
        sprintf(buf, _format_string, selected_item().label);
        LCD.print(buf);

        if (_blink)
        {
            LCD.setCursor(_col, _row);
            LCD.blink();
        }

    }

    void set_format(const char *format) {
        strcpy(_format_string, format);
    }

    SelectionItem<T> selected_item(){
        SelectionItem<T> item;
        memcpy_P(&item, &items[selected], sizeof(SelectionItem<T>));
        return item;
    }

private:
    const SelectionItem<T> *items;
    uint8_t num_of_items;

    uint8_t _col;
    uint8_t _row;
    char _format_string[7] = "%5s";
    boolean _blink;
};

#endif
