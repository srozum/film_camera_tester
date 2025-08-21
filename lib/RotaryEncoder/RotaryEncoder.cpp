#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(uint8_t CLK, uint8_t DA) : pinCLK(CLK), pinDA(DA) {}

void RotaryEncoder::init()
{
    pinMode(pinCLK, INPUT);
    pinMode(pinDA, INPUT);

    OCR0B = 0xF9;
}

void RotaryEncoder::start(void)
{
    last = read();
    delta = 0;
    TIMSK0 |= _BV(OCIE0B); // enable timer compare interrupt
}

void RotaryEncoder::stop(void)
{
    TIMSK0 &= ~(_BV(OCIE0B)); // disable timer compare interrupt
}

void RotaryEncoder::update(void)
{
    int8_t curr = read();

    int8_t diff = last - curr;

    if (diff & 1)
    {
        last = curr;
        delta += (diff & 2) - 1;
    }
}

int8_t RotaryEncoder::getValue(void)
{
    int8_t val;

    cli();
    val = delta;
    delta = val & 3;
    sei();

    val >>= 2;

    if (val < 0)
    {
        return -1;
    }
    else if (val > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int8_t RotaryEncoder::read(void)
{
    int8_t r = 0;

    if ((*portInputRegister(digitalPinToPort(pinCLK)) & digitalPinToBitMask(pinCLK)) == LOW)
    {
        r = 3;
    }

    if ((*portInputRegister(digitalPinToPort(pinDA)) & digitalPinToBitMask(pinDA)) == LOW)
    {
        r ^= 1;
    }

    return r;
}
