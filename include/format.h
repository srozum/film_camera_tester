/*
    Film Camera Tester
    Copyright (C) 2021  Serhiy Rozum
    Formatting helpers
*/

#pragma once

#include <Arduino.h>
#include "config.h"
#include "utills.h"
#include "settings.h"
#include "selection_items.h"

namespace Format
{

    // remove leading or trailing spaces
    void ltrim(char *str)
    {
        while (str[0] == 32) // last occurence of space char
        {
            memmove(str, str + 1, strlen(str));
        }
    }

    // remove trailing spaces
    void rtrim(char *str)
    {
        char *space;
        while ((space = strrchr(str, 32))) // last occurence of space char
        {
            *space = '\x0';
        }
    }

    // Format time (max 7 chars)
    void time(float &time, char *str)
    {
        float t = time;
        if (t < 1000.0)
        {
            // microseconds
            dtostrf(t, -5, 0, str);
            Format::rtrim(str);
            strcat(str, "\xE4s");
        }
        else
        {
            t = t / 1000.0;
            // milliseconds
            if (t < 100.0)
            {
                dtostrf(t, -5, 2, str);
                Format::rtrim(str);
                strcat(str, "ms");
            }
            else if (t < 700.0)
            {
                dtostrf(t, -5, 1, str);
                Format::rtrim(str);
                strcat(str, "ms");
            }
            else
            {
                t = t / 1000.0;
                // seconds
                dtostrf(t, -6, 2, str);
                Format::rtrim(str);
                strcat(str, "s");
            }
        }
    }

    // Format speed (max 5 chars)
    void speed(float &time, char *str)
    {
        float t = time / 1000.0;
        if (t > 700.0)
        {
            dtostrf(t / 1000.0, -4, 2, str);
            Format::rtrim(str);
            strcat(str, "s");
            return;
        }
        if (t > 99.9)
        {
            dtostrf((1000.0 / t), 5, 1, str);
        }
        else
        {
            dtostrf((1000.0 / t), 5, 0, str);
        }

        char *p;
        p = strrchr(str, 32);
        *p = '\x7';

        Format::ltrim(str);
    }

    // EV Error string (sign, 0.0, EV = 6 chars)
    void ev_error(float &error, char *str, char prec = 1)
    {
        dtostrf(error, (3 + prec), prec, str);
        if (error > 0)
        {
            char *p;
            p = strrchr(str, 32);
            *p = '+';
        }
        Format::ltrim(str);
        strcat(str, "EV");
    }

}
