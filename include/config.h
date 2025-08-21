/*
    Film Camera Tester
    Copyright (C) 2021  Serhiy Rozum
*/

#pragma once

#ifndef _CONFIG_H
#define _CONFIG_H

// Constants

#define SRT_VERSION_MAJOR 2
#define SRT_VERSION_MINOR 9

#define NOT_IN_USE 255

#define MENU_SIZE 4

#define ENCODER_DELAY 100

const char ModeSimpleName[] PROGMEM = "Simple Shutters";
const char ModeFocalVerticalName[] PROGMEM = "Focal Vertical";
const char ModeFocalHorizontalName[] PROGMEM = "Focal Horizontal";
const char ModeLightSourceName[] PROGMEM = "Light Source";
const char ModeExposureName[] PROGMEM = "Exposure Error";
const char ModeSeriesSimpleName[] PROGMEM = "Series Average";
const char ModeFlashSimpleName[] PROGMEM = "Flash Sync Simple";
const char ModeFlashFocalName[] PROGMEM = "Flash Sync Focal";
const char ModeLeafEfficiencyName[] PROGMEM = "Leaf Efficiency";

const char ShortNameSimple[] PROGMEM = "Simple";
const char ShortNameFocalVertical[] PROGMEM = "FP Vert";
const char ShortNameFocalHorizontal[] PROGMEM = "FP Horiz";
const char ShortNameLight[] PROGMEM = "Light";
const char ShortExposure[] PROGMEM = "Exposure";
const char ShortNameSeries[] PROGMEM = "Average";
const char ShortNameFlashSimple[] PROGMEM = "Flash Simple";
const char ShortNameFlashFocal[] PROGMEM = "Flash Focal";
const char ShortNameLeafEff[] PROGMEM = "Leaf Eff";

// Pins map of SRT version 2
// Based on Arduino Nano

#define BUZZER_PIN 13

#define BTN_MODE_PIN A0 // Mode button
#define BTN_MENU_PIN A1 // Menu button
#define BTN_START_PIN 7 // Start button
#define BTN_STOP_PIN 6  // Stop button, have to be on same register as Socket A

#define ENCODER_CLK_PIN 12
#define ENCODER_DA_PIN 11
#define ENCODER_SWITCH_PIN 8

// Socket A
#define SOCKET_A_ID_PIN A6 // Have to be any analog pin
#define SOCKET_A_SENSOR_PIN_1 3
#define SOCKET_A_SENSOR_PIN_2 4
#define SOCKET_A_SENSOR_PIN_3 5

// Socket B
#define SOCKET_B_ID_PIN A7 // Have to be any analog pin
#define SOCKET_B_SENSOR_PIN_1 2

// Light Unit Socket
#define LIGHT_UNIT_ID_PIN A2 // Have to be any analog pin
#define LIGHT_UNIT_PIN_1 9   // Light PWM
#define LIGHT_UNIT_PIN_2 10  // Fan

#endif
