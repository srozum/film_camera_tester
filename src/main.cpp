/*

    Film Camera Tester
    Arduino based shield for measuring shutter speeds of film cameras,
    and testing light meters accuracy.

    Copyright (C) 2021  Serhiy Rozum

*/
#include <Arduino.h>
#include <Timer2.h>
#include <RotaryEncoder.h>
#include <Button.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD(0x27, 20, 4);

#include "config.h"
#include "utills.h"
#include "glyphs.h"
#include "settings.h"
#include "active_buzzer.h"
#include "selection_items.h"
#include "exposure.h"
#include "format.h"
#include "exposure_counter.h"

ActiveBuzzer Buzzer(BUZZER_PIN);

Button ModeButton(BTN_MODE_PIN);
Button MenuButton(BTN_MENU_PIN);
Button StartButton(BTN_START_PIN);
Button StopButton(BTN_STOP_PIN);

RotaryEncoder Encoder(ENCODER_CLK_PIN, ENCODER_DA_PIN);

ISR(TIMER0_COMPB_vect)
{
  Encoder.update();
}

#define ENC_BUTTONINTERVAL 50 // check button every x milliseconds
#define ENC_HOLDTIME 2000     // report held button after 3s or so

int enc_value = 0;
unsigned long enc_last_button_check = 0;
unsigned long enc_key_down_ticks = 0;

Button EncoderButton(ENCODER_SWITCH_PIN);

Timer2 Timer;

ISR(TIMER2_OVF_vect)
{
  Timer.increment_overflow_count();
}

ExposureCounter Counter;

ISR(INT0_vect)
{
  Counter.isr();
}

ISR(TIMER2_COMPA_vect)
{
  Counter.timer();
}

#include "device.h"
#include "sensor.h"
#include "light_unit.h"

#include "socket.h"
#include "sensor_socket.h"
#include "light_unit_socket.h"

#include "items_selector.h"

#include "result.h"
#include "results/result_simple.h"
#include "results/result_focal.h"
#include "results/result_flash_sync.h"
#include "results/result_leaf_efficiency.h"
#include "results/result_light_source.h"
#include "results/result_exposure.h"

#include "mode.h"
#include "modes/mode_simple.h"
#include "modes/mode_focal.h"
#include "modes/mode_flash_simple.h"
#include "modes/mode_flash_focal.h"
#include "modes/mode_leaf_efficiency.h"
#include "modes/mode_light_source.h"
#include "modes/mode_exposure.h"

#include "mode_manager.h"
#include "settings_menu.h"

void setup()
{

  LCD.init();
  LCD.backlight();

  Utills::ShowWelcomeScreen();

  Glyphs::init();

  Buzzer.init();

  ModeButton.init();
  MenuButton.init();
  StartButton.init();
  StopButton.init();

  Encoder.init();
  Encoder.start();
  EncoderButton.init();

  // Hardware Setup
  LightUnit2.register_pins(OUTPUT, OUTPUT, NOT_IN_USE); // #2 PWM
  LightUnit2.add_ev_table(LightUnit2K12EV);             // 0 - K 12.5
  LightUnit2.add_ev_table(LightUnit2K14EV);             // 1 - K 14.03

  LightSource.register_pins(LIGHT_UNIT_PIN_1, LIGHT_UNIT_PIN_2, NOT_IN_USE);
  LightSource.register_device(&LightUnit2);
  LightSource.init();

  // Light Source Cannot be hot-swapped, so we should detect and initialize it here
  unsigned long LightSourceCheckStart = millis();
  while ((LightSourceCheckStart + 1000) > millis())
  {
    LightSource.update();
  };

  // Socket A Sensors
  Sensor1.register_pins(INPUT, INPUT, INPUT);           // #1 V1
  Sensor2.register_pins(INPUT, INPUT, INPUT);           // #1 V2
  Sensor4.register_pins(NOT_IN_USE, INPUT, NOT_IN_USE); // #2 Dot
  Sensor5.register_pins(NOT_IN_USE, INPUT, OUTPUT);     // #3 Laser

  // Socket B Sensors
  Sensor6.register_pins(INPUT, NOT_IN_USE, NOT_IN_USE); // #6 Exposure
  Sensor7.register_pins(INPUT, NOT_IN_USE, NOT_IN_USE); // #7 Flash

  SocketA.register_pins(SOCKET_A_SENSOR_PIN_1, SOCKET_A_SENSOR_PIN_2, SOCKET_A_SENSOR_PIN_3);
  SocketA.register_device(&Sensor1);
  SocketA.register_device(&Sensor2);
  SocketA.register_device(&Sensor4);
  SocketA.register_device(&Sensor5);
  SocketA.init();

  SocketB.register_pins(SOCKET_B_SENSOR_PIN_1, NOT_IN_USE, NOT_IN_USE);
  SocketB.register_device(&Sensor6);
  SocketB.register_device(&Sensor7);
  SocketB.init();

  ModeMgr.init();

  delay(1000);
  LCD.clear();
}

void loop()
{
  // Check Sockets status
  SocketA.update();
  SocketB.update();

  if (ModeMgr.is_runing())
  {
    if (ModeMgr.is_ready())
    {
      ModeMgr.run();
    }
    else
    {
      ModeMgr.stop();
    }
    LCD.clear();
  }
  else
  {

    // Mode Parameter Selector if present

    enc_value = Encoder.getValue();

    if (enc_value < 0)
    {
      ModeMgr.selector_next();
    }
    else if (enc_value > 0)
    {
      ModeMgr.selector_prev();
    }

    // Mode Selection Menu
    if (ModeButton.pressed())
    {
      ModeMgr.select();
      LCD.clear();
    }

    // Settings Menu
    if (MenuButton.pressed())
    {
      Menu.select();
      ModeMgr.reload();
      LCD.clear();
    }

    // Start Measurement
    if (StartButton.pressed())
    {
      ModeMgr.start();
      LCD.clear();
    }

    // Calibration on Encoder button held for 3 sec
    if (EncoderButton.pressed())
    {

      if ((millis() - enc_last_button_check) > ENC_BUTTONINTERVAL)
      {
        enc_last_button_check = millis();

        enc_key_down_ticks++;

        if (enc_key_down_ticks > (ENC_HOLDTIME / ENC_BUTTONINTERVAL))
        {
          enc_key_down_ticks = 0;
          ModeMgr.calibrate();
          LCD.clear();
        }
      }
      else
      {
        enc_key_down_ticks = 0;
      }
    }

  }

  ModeMgr.update(); // Print
}