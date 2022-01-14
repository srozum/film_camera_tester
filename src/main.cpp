/*

    Film Camera Tester
    Arduino based shield for measuring shutter speeds of film cameras.

    Basic demo

*/
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD(0x27, 20, 4);

#define BTN_START_PIN 7
#define BTN_STOP_PIN 6

// Socket A
#define SOCKET_A_ID_PIN A6
#define SOCKET_A_SENSOR_PIN_1 3
#define SOCKET_A_SENSOR_PIN_2 4
#define SOCKET_A_SENSOR_PIN_3 5

void setup()
{
  // put your setup code here, to run once:
  LCD.init();
  LCD.backlight();
  LCD.clear();

  pinMode(BTN_START_PIN, INPUT);
  pinMode(BTN_STOP_PIN, INPUT);

  pinMode(SOCKET_A_ID_PIN, INPUT);
  pinMode(SOCKET_A_SENSOR_PIN_1, INPUT_PULLUP);
  pinMode(SOCKET_A_SENSOR_PIN_2, INPUT_PULLUP);
  pinMode(SOCKET_A_SENSOR_PIN_3, INPUT_PULLUP);
}

void loop()
{

  // put your main code here, to run repeatedly:
  if (analogRead(SOCKET_A_ID_PIN) < 1000)
  {
    LCD.setCursor(4, 1);
    LCD.print(F("Press  Start"));

    if (digitalRead(BTN_START_PIN) == LOW)
    {
      LCD.clear();

      LCD.setCursor(4, 1);
      LCD.print(F("Measuring"));

      unsigned long start;
      unsigned long stop;

      int measure = 2;

      do
      {
        if ((digitalRead(SOCKET_A_SENSOR_PIN_2) == LOW) && (measure == 2))
        {
          start = micros();
          measure--;
        }
        if ((digitalRead(SOCKET_A_SENSOR_PIN_2) == HIGH) && (measure == 1))
        {
          stop = micros();
          measure--;
        }

        if (digitalRead(BTN_STOP_PIN) == LOW)
        {
          measure = -1;
        }
      } while (measure > 0);

      LCD.clear();

      if ((measure == 0))
      {
        char buff[12];

        // Print measured time
        float time = (stop - start) / 1000.0;

        char time_string[8];
        dtostrf(time, 5, 2, time_string);

        sprintf(buff, "%5sms", time_string);

        LCD.setCursor(4, 1);
        LCD.print(buff);

        // Format and print measured time as exposure time
        float speed = 1.0 / (time / 1000.0);

        sprintf(buff, "1/%d", (int) speed);

        LCD.setCursor(4, 2);
        LCD.print(buff);

        delay(3000);
        LCD.clear();

      }
    }
  }
  else
  {
    LCD.setCursor(4, 1);
    LCD.print(F(" Not  Ready "));
  }
}