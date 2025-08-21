/*

  Exposure Counter - class to count and measure pulses of Light to Frequency Sensor

  Copyright (C) 2022  Serhiy Rozum

  Outside loop should periodically check available()
  When result is ready, oustide loop should read results, run reset() and then continue loop;

*/

#ifndef _EXPOSURE_COUNTER_H
#define _EXPOSURE_COUNTER_H

#include <Arduino.h>

extern volatile unsigned long timer0_overflow_count;

#define EXPOSURE_COUNTER_INTERVAL 200000L // 200 ms

#define EXPOSURE_CALIBRATION_INTERVAL 1000
#define EXPOSURE_CALIBRATION_TESTS 10

class ExposureCounter
{
public:
    volatile bool calibrating = false;
    volatile bool measuring = false;
    volatile unsigned long timestamp = 0;
    volatile unsigned long start_timestamp = 0;
    volatile unsigned long counts = 0;
    volatile unsigned long timer_ticks;

    unsigned long first_pulse_at = 0;
    unsigned long last_pulse_at = 0;
    unsigned long prev_counts = 0;

    ExposureCounter(){};

    void start()
    {
        reset();
        measuring = false;

        // page 54 of datasheet
        EICRA &= ~((1 << ISC00) | (1 << ISC01)); // clear existing flags
        EICRA |= (RISING << ISC00);              // set wanted flags (RASING level interrupt)
        EIFR = (1 << INTF0);                     // clear flag for interrupt 0
        EIMSK |= (1 << INT0);                    // enable it
    }

    void stop()
    {
        EIMSK &= ~(1 << INT0);
    }

    void reset()
    {
        cli();

        timestamp = micros();
        last_pulse_at = timestamp;
        counts = 0;
        prev_counts = 0;

        sei();
    }

    unsigned long calibrate()
    {
        unsigned long average_counts = 0;

        for (byte i = 0; i < EXPOSURE_CALIBRATION_TESTS; i++)
        {

            // Setup  timer
            TCCR2A = bit(WGM21);   // - CTC mode (WGM22:0 = 2)
            OCR2A = 124;           // count up to 125  (zero relative!!!!)
            TIMSK2 |= bit(OCIE2A); // Enable Timer2 compare match interrupt

            // Reset
            counts = 0;
            timer_ticks = 0; // reset interrupt counter
            calibrating = true;

            TCNT2 = 0;           // reset Timer2 counter
            TIFR2 |= bit(OCF2A); // reset overflow flag

            // Enable INT0
            EICRA &= ~((1 << ISC00) | (1 << ISC01)); // clear existing flags
            EICRA |= (RISING << ISC00);              // set wanted flags (RASING level interrupt)
            EIFR = (1 << INTF0);                     // clear flag for interrupt 0

            // Start
            TCCR2B = bit(CS20) | bit(CS22); // - Prescaler 128
            EIMSK |= (1 << INT0);           // enable interupt

            while (calibrating) {}

            average_counts += counts / EXPOSURE_CALIBRATION_TESTS;
        }

        return average_counts;
    }

    bool available()
    {

        // do not disturb interrupt too often
        if ((micros() - last_pulse_at) < EXPOSURE_COUNTER_INTERVAL)
            return false;

        cli();
        last_pulse_at = timestamp;
        unsigned long new_counts = counts;
        sei();

        if (measuring)
        {
            // still counting
            if (new_counts != prev_counts)
            {
                prev_counts = new_counts;
                return false;
            }

            // not fired yet
            if (!prev_counts)
            {
                return false;
            }

            // new == prev and !=0
            cli();
            measuring = false;
            last_pulse_at = timestamp;
            first_pulse_at = start_timestamp;
            sei();
            return true;
        }
        else
        {
            reset();
            measuring = true;
            return false;
        }
    }

    unsigned long time()
    {
        return last_pulse_at - first_pulse_at;
    }

    void isr()
    {
        if (calibrating)
        {
            counts++;
            return;
        }

        timestamp = ((timer0_overflow_count << 8) + TCNT0) * 4;

        if (measuring)
        {
            if (counts == 0)
                start_timestamp = timestamp;

            counts++;
        }
    }

    void timer()
    {

        if (++timer_ticks < EXPOSURE_CALIBRATION_INTERVAL)
            return;

        // stop counter
        EIMSK &= ~(1 << INT0);

        // stop timer 2
        TCCR2A = 0;
        TCCR2B = 0;
        TIMSK2 = 0;

        calibrating = false;
    }
};

#endif
