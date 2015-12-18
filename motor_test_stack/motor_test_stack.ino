#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMSbot = Adafruit_MotorShield(0x61); // Rightmost jumper closed
Adafruit_MotorShield AFMStop = Adafruit_MotorShield(0x60); // Default address, no jumpers

// On the top shield, connect two steppers, each with 200 steps
Adafruit_DCMotor *M1 = AFMStop.getMotor(3);
Adafruit_DCMotor *M2 = AFMStop.getMotor(4);

// On the bottom shield connect a stepper to port M3/M4 with 200 steps
Adafruit_DCMotor *M3 = AFMSbot.getMotor(1);
// And a DC Motor to port M1
//Adafruit_DCMotor *M4 = AFMSbot.getMotor(4);

void setup() {
  while (!Serial);
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("MMMMotor party!");

  //AFMSbot.begin(); // Start the bottom shield
  AFMStop.begin(); // Start the top shield
   
  // turn on the DC motor
  M1->setSpeed(40);
  M2->setSpeed(40);
  M3->setSpeed(40);
}

int i;
void loop() {
  M1->run(FORWARD);
  M2->run(FORWARD);
  M3->run(FORWARD);
}
