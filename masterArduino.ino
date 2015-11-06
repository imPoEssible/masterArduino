#include <Wire.h>
#include <Adafruit_MotorShield.h>

// set up LEDs
int light_mode = 0;
byte red_pin = 3;
byte blue_pin = 5;
byte green_pin = 6;
byte photoDiode_pin = A5;

int brightness;
long prevTime = 0;

// set up motors
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor3 = AFMS.getMotor(3);
Adafruit_DCMotor *myMotor4 = AFMS.getMotor(4);

byte motorSpeed1 = 30;
byte motorSpeed2 = 30;
byte motorSpeed3 = 30;
byte motorSpeed4 = 30;

// set up mode variable
byte mode = 0;

void setup() {
  pinMode(red_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(photoDiode_pin, INPUT);
  Serial.begin(9600);
  
  // Initialize Motor
  AFMS.begin();
  myMotor1->setSpeed(motorSpeed1);
  myMotor2->setSpeed(motorSpeed2);
  myMotor3->setSpeed(motorSpeed3);
  myMotor4->setSpeed(motorSpeed4);
  myMotor1->run(FORWARD);
  myMotor2->run(FORWARD);
  myMotor3->run(FORWARD);
  myMotor4->run(FORWARD);
}

void loop() {
  if (Serial.available() > 0){
    mode = Serial.read() - 48;
  }
  switch(mode){
    case 1:
      serialFlush();
      resetMotorLED();
      wave();
      serialFlush();
      resetMotorLED();
      mode = 0;
      break;
    default:
      if ((millis() - prevTime) > 1000){
        brightness = map(analogRead(photoDiode_pin), 500, 900, 0, 255);
        prevTime = millis();
      }
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
  
  //reset motors
  motorSpeed1 = 30;
  motorSpeed2 = 30;
  motorSpeed3 = 30;
  motorSpeed4 = 30;
  
  myMotor1->setSpeed(motorSpeed1);
  myMotor2->setSpeed(motorSpeed2);
  myMotor3->setSpeed(motorSpeed3);
  myMotor4->setSpeed(motorSpeed4);
  
  myMotor1->run(FORWARD);
  myMotor2->run(FORWARD);
  myMotor3->run(FORWARD);
  myMotor4->run(FORWARD);
}

void wave(){
  prevTime = millis();
  motorSpeed4 = 100;
  myMotor4->setSpeed(motorSpeed4);
  while ((millis() - prevTime) < 3000){
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE);
    myMotor3->run(RELEASE);
    myMotor4->run(FORWARD);
  }
}
