#include <Arduino.h>

#include "camera.h"

#define NUMCHARS 128
char receivedChars[NUMCHARS];
boolean newData = false;

HardwareSerial &myCamera = Serial3;

void initCamera()
{
  myCamera.begin(9600);
}

/*
   The camera serial stream is documented - http://jevois.org/doc/UserSerialStyle.html

   For our case, the camera cannot be in Terse mode because we want the item descriptor.
   Sample output:  ﻿N2 win.png -22297 -11881 43244 22800

  I should clear the buffer before reading 
  
   OUTPUT a text string describing what the reported object is, i.e. the second token
*/
char *readCamera()
{
  char *pch;

  recvWithStartEndMarkers();

  if (newData == true)
  {
#ifdef DEBUG
    Serial.print("SERIAL: (");
    Serial.print(receivedChars);
    Serial.print(") Parsing ");
#endif
    pch = strtok(receivedChars, " ,."); // String Type
                                        // I should check if this is really N2

#ifdef DEBUG
    Serial.print("Message Type : ");
    Serial.print(pch);
#endif

    pch = strtok(NULL, " , . - "); // This is what I saw
#ifdef DEBUG
    Serial.print(" Contents: ");
    Serial.print(pch);
    // Nothing else is intereting
    Serial.println(" EOM");
#endif

    newData = false;

#ifdef DEBUG
    Serial.println("Camera Flush");
#endif
    cameraFlush();
    return pch;
  }
#ifdef DEBUG
  Serial.println("Camera says no data");
#endif

  return NULL;
}

/*
     Manipulate the serial port
*/

// Example 3 - Receive with start- and end-markers
// https://forum.arduino.cc/index.php?topic=396450.0

void recvWithStartEndMarkers()
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = 'N';
  char endMarker = '\n';

  while (myCamera.available() > 0 && newData == false)
  {
    char rc;

    rc = myCamera.read();

    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
#ifdef DEBUG
        //Serial.print(receivedChars[ndx]);
#endif
        ndx++;
        if (ndx >= NUMCHARS)
        {
          ndx = NUMCHARS - 1;
#ifdef DEBUG
          Serial.print("Overflow");
#endif
        }
      }
      else
      {
#ifdef DEBUG
        Serial.print(") End Marker!  ");
        Serial.println(ndx);
#endif
        receivedChars[ndx] = '\0'; // terminate the string and replaces the End Marker with NULL
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker)
    {
      recvInProgress = true;
      receivedChars[ndx] = rc;
#ifdef DEBUG
      Serial.print("Start Marker! (");
      Serial.print(receivedChars[ndx]);
#endif
      ndx++;
    }
  }
}

void cameraFlush()
{
  while (myCamera.available() > 0)
  {
    myCamera.read();
  }
}