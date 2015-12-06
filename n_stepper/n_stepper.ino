#include <Stepper.h>

const int mSpeed = 60;
const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
const int mSpeed1 = 3000;
const int mSpeed2 = 3000;
const int stepsPerRevolution1 = 5;  // change this to fit the number of steps per revolution
const int stepsPerRevolution2 = 5;  // change this to fit the number of steps per revolution

// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper M1(stepsPerRevolution1, 22, 23, 24, 25);
//Stepper M1(stepsPerRevolution1, 4,5,6,7);
Stepper M2(stepsPerRevolution2, 26, 27, 28, 29);
Stepper M3(stepsPerRevolution, 30, 31, 32, 33);
Stepper M4(stepsPerRevolution, 34, 35, 36, 37);
Stepper M5(stepsPerRevolution, 38, 39, 40, 41);
Stepper M6(stepsPerRevolution, 42, 43, 44, 45);
Stepper M7(stepsPerRevolution, 46, 47, 48, 49);
Stepper M8(stepsPerRevolution, 50, 51, 52, 53);

//int count;

void setup() {
  // set the speed at 60 rpm:
  M1.setSpeed(mSpeed1);
  M2.setSpeed(mSpeed2);
  M3.setSpeed(mSpeed);
  M4.setSpeed(mSpeed);
  M5.setSpeed(mSpeed);
  M6.setSpeed(mSpeed);
  M7.setSpeed(mSpeed);
  M8.setSpeed(mSpeed);
  // initialize the serial port:
  //Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
//  Serial.println("clockwise");
  M1.step(stepsPerRevolution1);
  M2.step(stepsPerRevolution2);
  // step one revolution in the other direction:
//  Serial.println("counterclockwise");
//  count = 0;
//  while (count < 500){
//    count ++;
//    M1.step(-stepsPerRevolution);
//    M2.step(-stepsPerRevolution);
//  }
//  delay(500);
}
