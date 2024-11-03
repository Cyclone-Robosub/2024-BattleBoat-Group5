/*
   Gamepad module provides three different mode namely Digital, JoyStick and Accerleometer.

   You can reduce the size of library compiled by enabling only those modules that you want to
   use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/game-pad-module/
*/
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

const int rightIn1 = 6;
const int rightIn2 = 7;
const int rightPwm = 10;

const int leftIn1 = 4;
const int leftIn2 = 5;
const int leftPwm = 9;


enum Direction {FORWARDS, BACKWARDS};

void setup() {
  // put your setup code here, to run once:
  
  pinMode(rightIn1, OUTPUT);
  pinMode(rightIn2, OUTPUT);
  pinMode(rightPwm, OUTPUT);
  pinMode(leftIn1, OUTPUT);
  pinMode(leftIn2, OUTPUT);
  pinMode(leftPwm, OUTPUT);

  digitalWrite(rightIn1, HIGH);
  digitalWrite(rightIn2, LOW);

  digitalWrite(leftIn1, LOW);
  digitalWrite(leftIn2, HIGH);

  analogWrite(rightPwm, 0);
  analogWrite(leftPwm, 0);

  Serial.begin(250000);      // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin(9600);      //Enter baudrate of your bluetooth. Connect bluetooth on Bluetooth port present on evive.
}

void setLeftMotorPower(int pwm, Direction direction) {
  Serial.print("left: ");
  Serial.print(pwm);
  Serial.print('\t');

  switch (direction) {
    case FORWARDS:
      digitalWrite(rightIn1, LOW);
      digitalWrite(rightIn2, HIGH);
      break;
    case BACKWARDS:
      digitalWrite(rightIn1, HIGH);
      digitalWrite(rightIn2, LOW);
      break;
    default:
      Serial.print("Error: Impossible direction!\n");
  }
  analogWrite(rightPwm, pwm);
}

void setRightMotorPower(int pwm, Direction direction) {
  Serial.print("right: ");
  Serial.print(pwm);
  Serial.print('\n');

  switch (direction) {
    case FORWARDS:
      digitalWrite(leftIn1, HIGH);
      digitalWrite(leftIn2, LOW);
      break;
    case BACKWARDS:
      digitalWrite(leftIn1, LOW);
      digitalWrite(leftIn2, HIGH);
      break;
    default:
      Serial.print("Error: Impossible direction!\n");
  }
  analogWrite(leftPwm, pwm);
}

void loop() {
  Dabble.processInput();            //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  static int leftMotorPwm = 0;
  static int rightMotorPwm = 0; 
  static Direction leftMotorDirection = FORWARDS;
  static Direction rightMotorDirection = FORWARDS;

  float y = (GamePad.getYaxisData()/6) * 255;

  float x = (GamePad.getXaxisData()/6) * 255;

  if (x > 255) {
    x = 255;
  }
  if (x < -255) {
    x = -255;
  }
  if (y > 255) {
    y = 255;
  }
  if (y < -255) {
    y = -255;
  }


  // Serial.print("x_axis: ");
  // Serial.print(x);
  // Serial.print('\t');

  // Serial.print("y_axis: ");
  // Serial.println(y);
  // Serial.println();

  leftMotorPwm = (int)(y + x);
  rightMotorPwm = (int)(y - x);

  if (leftMotorPwm < 0) {
    leftMotorDirection = BACKWARDS;
    leftMotorPwm *= -1;
  }
  else {
    leftMotorDirection = FORWARDS;
  }

  if (rightMotorPwm < 0) {
    rightMotorDirection = BACKWARDS;
    rightMotorPwm *= -1;
  }
  else {
    rightMotorDirection = FORWARDS;
  }

  if (leftMotorPwm > 255) {
    leftMotorPwm = 255;
  }
  if (rightMotorPwm > 255) {
    rightMotorPwm = 255;
  }

  setLeftMotorPower(leftMotorPwm, leftMotorDirection);
  setRightMotorPower(rightMotorPwm, rightMotorDirection);


}
