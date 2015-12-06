#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Stepper.h>

//MOTOR CONSTANTs:
const int mSpeed = 60;
const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
const int mSpeed1 = 3000;
const int mSpeed2 = 3000;
const int stepsPerRevolution1 = 5;  // change this to fit the number of steps per revolution
const int stepsPerRevolution2 = 5;  // change this to fit the number of steps per revolution

// initialize the stepper library on pins 8 through 11:
Stepper M1(stepsPerRevolution1, 22, 23, 24, 25);
Stepper M2(stepsPerRevolution2, 26, 27, 28, 29);

// set up LEDs
int light_mode = 0;
byte red_pin = 3;
byte blue_pin = 5;
byte green_pin = 6;
byte photoDiode_pin = A5;

int brightness;
long prevTime = 0;


// set up mode variable
byte mode = 0;

void setup() {
  M1.setSpeed(mSpeed1);
  M2.setSpeed(mSpeed2);

  pinMode(red_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(photoDiode_pin, INPUT);
  Serial.begin(9600);
  
}

void loop() {
  if (Serial.available() > 0){
    mode = Serial.read() - 48;
  }
  switch(mode){
    case 1:
      serialFlush();
      resetMotorLED();
      M1.step(stepsPerRevolution1);
      M2.step(stepsPerRevolution2);
      //mode = 0;
      break;
    default:
      if ((millis() - prevTime) > 1000){
        brightness = map(analogRead(photoDiode_pin), 500, 900, 0, 255);
        prevTime = millis();
      }
      Serial.println("default");
      analogWrite(red_pin, brightness);
//      analogWrite(blue_pin, brightness);
//      analogWrite(green_pin, brightness);
      break;
  }
}

void serialFlush(){
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void resetMotorLED(){
  //reset LEDs
  analogWrite(red_pin, 0);
  analogWrite(blue_pin, 0);
  analogWrite(green_pin, 0);
  
}
