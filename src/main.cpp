/* ***************************************************************************************
   System to sort brass by headstamp - Demo

   Demonstration of the basic components to sort brass by headstamp include:
    - Stepper motor to move cases along a track
    - Camera interface (JeVois) to identify headstamps
    - Servo to move gates to sort selected items

    Components
    - Arduino Mega
    - Stepper Motor and controller
    - JeVois camera
    - Servo
    - LCD -  http://wiki.sunfounder.cc/index.php?title=I2C_LCD2004#Step_2:Add_library

 ***************************************************************************************** */
#include <Arduino.h>
#include "camera.h"
#include "display.h"

//Servos
#define SERVO_1_CNTRL 4 // Unused

//Stepper
#define STEPPER_IN4 10
#define STEPPER_IN3 11
#define STEPPER_IN2 12
#define STEPPER_IN1 13

#include <Servo.h>

/*
   Twelve servo objects can be created on most boards
*/
Servo myservo; // create servo object to control a servo

#define SERVO_NONE 0 // Degrees of rotation for servo to be OPEN
#define SERVO_A 60   // Degrees of rotation for servo to be OPEN
#define SERVO_B 120  // Degrees of rotation for servo to be OPEN
#define SERVO_C 180  // Degrees of rotation for servo to be OPEN

/*
   For specs and how to use the stepper, see
   https://www.seeedstudio.com/blog/2019/03/04/driving-a-28byj-48-stepper-motor-with-a-uln2003-driver-board-and-arduino/
*/

/* Consider using - http://www.airspayce.com/mikem/arduino/AccelStepper/
*/
#include <Stepper.h>
const int stepsPerRevolution = 2038; // change this to fit the number of steps per revolution
const int rolePerMinute = 15;        // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, STEPPER_IN4, STEPPER_IN2, STEPPER_IN3, STEPPER_IN1);

/*
   Software Serial Port
     - https://www.arduino.cc/en/Reference/SoftwareSerial
     - I read somewhere that the UNO can onlu support interupts on pin 2 and 3 so I put the serial port here.

   For the JeVois camera.  Camera is configured for 9600 baud
     - http://jevois.org/doc/UserSerial.html
     - http://jevois.org/tutorials/UserArduinoBlink.html


*/

void setup()
{
  // Initialize the Serial and LCD interfaces for status messages
  // Do this first so we can see what is going on
  Serial.begin(9600);
  initDisplay();

  // Setup and initialize any data structures
  Serial.println("Initialize data structures");

  // Initialize Serial interface for Camera messages
  Serial.println("Initialize Camera");

  initCamera();

  // Initialize Stepper motor
  //  Should this also align the belt with some sort of optical sensor?
  Serial.println("Initialize Stepper motor");
  myStepper.setSpeed(rolePerMinute);

  // Initialize and pre-position servos
  Serial.println("Initialize Servo");
  myservo.attach(SERVO_1_CNTRL); // attaches the servo to the servo object
  myservo.write(SERVO_NONE);     // Set servo to "Closed"
}
char *type_p; // Pointer to a string that is the name of the object seen by the camera
char buffer[21];

void loop()
{
  // Read the camera and see what type of case we have
  message(OPERATION_MESSAGE, "1 - Read Case Type");
  delay(1000);

  if ((type_p = readCamera()) != NULL)
  {
    sprintf(buffer, "Type %s\n", type_p);
    Serial.print(buffer);
    message(DATA_MESSAGE, buffer);
    message(OPERATION_MESSAGE, "2 - Position Servo");

    // This only works because the names I chose for testing are single characters.  It would be nice if it matched against strings
    switch (*type_p)
    {
    case 'a':
      myservo.write(SERVO_A);
      break;

    case 'b':
      myservo.write(SERVO_B);
      break;

    case 'c':
      myservo.write(SERVO_C);
      break;

    default:
      myservo.write(SERVO_NONE);
      break;
    }

    delay(1000); // waits for the servo to reach the position, return to zero, then wait again for it to reach position
    myservo.write(SERVO_NONE);
    delay(1000);
  }
  else
  {
    message(OPERATION_MESSAGE, "2 - No Case Detected");
    delay(1000); // waits 15ms for the servo to reach the position
  }

  // Position next case
  message(OPERATION_MESSAGE, "3 - Next Case");
  clearMessageLine(DATA_MESSAGE);
  myStepper.step(-stepsPerRevolution);
  delay(1500);

  cameraFlush();
}
