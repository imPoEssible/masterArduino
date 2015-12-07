#include <Wire.h>
#include <Adafruit_MotorShield.h>

//MOTOR CONSTANTs:
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *M1 = AFMS.getMotor(1);
Adafruit_DCMotor *M2 = AFMS.getMotor(2);

int M1speed = 40; //set motor speeds
int M2speed = 40;


// set up LEDs
int light_mode = 0;
byte red_pin = 3;
byte blue_pin = 5;
byte green_pin = 6;
byte photoDiode_pin = A5;

int brightness;
long prevTime = 0;
long offsetTime = 0; //Used to create offset

// set up mode variable
byte mode = 0;

void setup() {
  AFMS.begin();
  M1->setSpeed(M1speed);
  M2->setSpeed(M2speed);

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
      offsetTime = millis();
      while (millis() - offsetTime<10){
        M1->run(BACKWARD);
        offsetTime=0; //Reseting it
      }
      M1->run(BACKWARD);
      M2->run(BACKWARD);
      break;
    case 2:
      serialFlush();
      
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

void resetMotor(){
  //reset motors
  M1->run(RELEASE);
  M2->run(RELEASE);
}
