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


// set up mode variable
byte mode = 0;

void setup() {
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
  
}
