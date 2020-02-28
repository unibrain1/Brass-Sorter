#include <Arduino.h>
#include "display.h"

// LiquidCrystal_I2C display(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

#define LINE_LENGTH 20
LiquidCrystal_I2C display = LiquidCrystal_I2C(0x27, LINE_LENGTH, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

void initDisplay()
{
  display.init();      //initialize the lcd
  display.backlight(); //open the backlight
}
void message(int message_type, String text)
{
    clearMessageLine(message_type);
    display.setCursor(0, message_type);
    display.print(text);
   
}

void clearMessageLine(int line)
{
  display.setCursor(0, line);
  for (int n = 0; n < LINE_LENGTH; n++) // 20 indicates symbols in line. For 2x16 LCD write - 16
  {
    display.print(" ");
  }
}
