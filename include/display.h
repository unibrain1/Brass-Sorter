/*
 *
 */

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

// include the library code
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define OPERATION_MESSAGE 0
#define DATA_MESSAGE 1
#define STATUS_MESSAGE 2
#define DEBUG_MESSAGE 3

void message(int type, String text);
void initDisplay();

void clearMessageLine(int line);

#endif

