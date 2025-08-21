/*
    Film Camera Tester
    Copyright (C) 2021  Serhiy Rozum
    Custom Glyphs
*/

#pragma once

#include <Arduino.h>
#include "config.h"

enum class Glyph : uint8_t
{
    NONE = 32,
    DOT = 1,
    LASER = 213,
    FOCAL = 2,
    FLASH = 3,
    PLUSONE = 4,
    EXPOSURE = 42,
    ARR_RIGHT = 126,
    SPEED = 7,
};

namespace Glyphs
{

    const PROGMEM uint8_t DOT[8] = {B00000, B00000, B00100, B01110, B00100, B00000, B00000, B00000};
    const PROGMEM uint8_t FOCAL[8] = {B00000, B00001, B00000, B00100, B00000, B10000, B00000, B00000};
    const PROGMEM uint8_t FLASH[8] = {B00011, B00110, B01100, B11111, B00010, B00100, B01000, B00000};
    const PROGMEM uint8_t PLUSONE[8] = { B01001, B11101, B01001, B00000, B00000, B00000, B00000, B00000};
    const PROGMEM uint8_t SPEED[8] = {B01000, B11000, B01001, B01010, B00100, B01000, B10000, B00000};

    void createGlyph(Glyph id, const uint8_t *bitmap)
    {
        uint8_t data[8];
        memcpy_P(&data, bitmap, 8);
        LCD.createChar((uint8_t)id, data);
    }

    void init()
    {
        createGlyph(Glyph::DOT, DOT);               // small plus at center, Dot Sensor
        createGlyph(Glyph::FOCAL, FOCAL);           // small diagonal dots, Diagonal Sensor V2
        createGlyph(Glyph::FLASH, FLASH);           // flash
        createGlyph(Glyph::PLUSONE, PLUSONE);       // +1
        createGlyph(Glyph::SPEED, SPEED);           // Dot in circle, leaf shutter mode
    }

}
