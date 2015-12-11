#include <Wire.h>
#include <Adafruit_MotorShield.h>
//MOTOR SETUP:
Adafruit_MotorShield AFMS_bot = Adafruit_MotorShield(0x60); // Default address, no jumpers
Adafruit_MotorShield AFMS_top = Adafruit_MotorShield(0x61); // Rightmost jumper closed

Adafruit_DCMotor *M1 = AFMS_bot.getMotor(1);
Adafruit_DCMotor *M2 = AFMS_bot.getMotor(2);
Adafruit_DCMotor *M3 = AFMS_bot.getMotor(3);
Adafruit_DCMotor *M4 = AFMS_bot.getMotor(4);
Adafruit_DCMotor *M5 = AFMS_top.getMotor(5);
Adafruit_DCMotor *M6 = AFMS_top.getMotor(6);
Adafruit_DCMotor *M7 = AFMS_top.getMotor(7);
Adafruit_DCMotor *M8 = AFMS_top.getMotor(8);

int M1_s = 40; //set motor speeds
int M2_s = 40;
int M3_s = 40;
int M4_s = 40;
int M5_s = 40;
int M6_s = 40;
int M7_s = 40;
int M8_s = 40;

//LED SETUP
// You can choose the latch pin yourself.
const int ShiftPWM_latchPin=8;
const bool ShiftPWM_invertOutputs = false;
const bool ShiftPWM_balanceLoad = false;

#include <ShiftPWM.h>   // include ShiftPWM.h after setting the pins!

// Function prototypes (telling the compiler these functions exist).
void breatheSleep(void);
void oneByOne(void);
void inOutTwoLeds(void);
void inOutAll(void);
void alternatingColors(void);
void hueShiftAll(void);
void randomColors(void);
void rgbLedRainbow(unsigned long cycleTime, int rainbowWidth);

// Here you set the number of brightness levels, the update frequency and the number of shift registers.
// These values affect the load of ShiftPWM.
// Choose them wisely and use the PrintInterruptLoad() function to verify your load.
unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
unsigned int numRegisters = 3;
unsigned int numOutputs = numRegisters*8;
unsigned int numRGBLeds = numRegisters*8/3;
unsigned int fadingMode = 0; //start with all LED's off.

unsigned long startTime = 0; // start time for the chosen fading mode

//Photodiode
byte photoDiode_pin = A0;
long pd_bright;
long offsetTime = 0; //Used to create offset

// set up mode variable
byte mode = 0;
byte prev_mode = 0;

//KNOCK VARIABLES
byte knock_pin = A1;
int state = 0; // Active = 1, Passive = 0
int counter = 0; // counts knocks up to 3
int impactval;
long prevTime = 0;

void setup() {
  AFMS_top.begin();
  AFMS_bot.begin();
  M1->setSpeed(M1_s);
  M2->setSpeed(M2_s);
  M3->setSpeed(M3_s);
  M4->setSpeed(M4_s);
  M5->setSpeed(M5_s);
  M6->setSpeed(M6_s);
  M7->setSpeed(M7_s);
  M8->setSpeed(M8_s);

  // Sets the number of 8-bit registers that are used.
  ShiftPWM.SetAmountOfRegisters(numRegisters);

  // SetPinGrouping allows flexibility in LED setup. 
  // If your LED's are connected like this: RRRRGGGGBBBBRRRRGGGGBBBB, use SetPinGrouping(4).
  ShiftPWM.SetPinGrouping(8);
  ShiftPWM.Start(pwmFrequency,maxBrightness);
  
  Serial.begin(9600);
  
}

void loop() {
  //CHANGE NUMBER AS NECESSARY
  if ((millis()-prevTime)> 1000){
    counter = 0;
  }
  
  if (Serial.available() > 0){
    mode = Serial.read() - 48;
    startTime = millis();
  }
//  if (mode == prev_mode){
//    continue;
//  }
//  pd_bright = map(analogRead(photoDiode_pin), 690, 1000, 0, 1);
  pd_bright = map(analogRead(photoDiode_pin), 500, 900, 0, 1);

  detectKnock();
  if (counter == 1){
    mode = 3;
  }

  switch(mode){
    case 1:
      serialFlush();
      wave1();
      inOutAll();
      break;
    case 2:
      serialFlush();
      oneByOne();
      resetMotor();      
      break;
    case 3:
      resetMotor();
      M1->run(FORWARD);
      randomColors();
      break;
    case 4:
      resetMotor();
      M2->run(FORWARD);
      rgbLedRainbow(3000,numRGBLeds);
    default:
      breatheSleep();
      break;
  }
}

void serialFlush(){
  while (Serial.available() > 0) {
    Serial.read();
  }
}

/*MOTOR FUNCTIONS*/
void resetMotor(){
  //reset motors
  M1->run(RELEASE);
  M2->run(RELEASE);
  M3->run(RELEASE);
  M4->run(RELEASE);
  M5->run(RELEASE);
  M6->run(RELEASE);
  M7->run(RELEASE);
  M8->run(RELEASE);

  M1_s = 40; //set motor speeds
  M2_s = 40;
  M3_s = 40;
  M4_s = 40;
  M5_s = 40;
  M6_s = 40;
  M7_s = 40;
  M8_s = 40;
  
  M1->setSpeed(M1_s);
  M2->setSpeed(M2_s);
  M3->setSpeed(M3_s);
  M4->setSpeed(M4_s);
  M5->setSpeed(M5_s);
  M6->setSpeed(M6_s);
  M7->setSpeed(M7_s);
  M8->setSpeed(M8_s);
}

void wave1(){
  unsigned long cycleTime = 5000;
  unsigned long time = millis()-startTime;
  
  M3_s = 1000;
  M4_s = 1000;
  
  M3->setSpeed(M3_s);
  M4->setSpeed(M4_s);
  
  if (time < cycleTime){
    M3->run(FORWARD);
    M4->run(FORWARD);
  }
  else{
    mode = 2;
  }

}

void alternate(){
}

/*ACCELEROMETER FUNCTIONS*/
void detectKnock(){
  impactval = analogRead(knock_pin);
  if (impactval > 500){   
    if ((millis() - prevTime) > 400){
      prevTime = millis();
      counter = counter + 1;
    }
    if (counter == 3){
      if (state == 0){
        counter = 0;
        state = 1;
      }
      else if (state == 1){
        counter = 0;
        state = 0;
      }
      }
    Serial.println(counter);
}
}

/*LED PWM FUNCTIONS BELOW*/
void breatheSleep(void){
  unsigned long cycleTime = 2000;
  unsigned char brightness;
  unsigned long time = millis()-startTime;
  unsigned long currentStep = time%(cycleTime*2);  
  unsigned long hue = (360*time/20000)%360;

  if(currentStep <= cycleTime ){
    brightness = currentStep*255/cycleTime; ///fading in
  }
  else{
    brightness = 255-(currentStep-cycleTime)*255/cycleTime; ///fading out;
  }
  
  ShiftPWM.SetAllHSV(hue, brightness, brightness); 
}

void oneByOne(void){ // Fade in and fade out all outputs one at a time
  unsigned char brightness;
  unsigned long fadeTime = 500;
  unsigned long loopTime = numOutputs*fadeTime*2;
  unsigned long time = millis()-startTime;
  unsigned long timer = time%loopTime;
  unsigned long currentStep = timer%(fadeTime*2);

  int activeLED = timer/(fadeTime*2);

  if(currentStep <= fadeTime ){
    brightness = currentStep*maxBrightness/fadeTime; ///fading in
  }
  else{
    brightness = maxBrightness-(currentStep-fadeTime)*maxBrightness/fadeTime; ///fading out;
  }
  ShiftPWM.SetAll(0);
  ShiftPWM.SetOne(activeLED, brightness);
}

void inOutTwoLeds(void){ // Fade in and out 2 outputs at a time
  unsigned long fadeTime = 500;
  unsigned long loopTime = numOutputs*fadeTime;
  unsigned long time = millis()-startTime;
  unsigned long timer = time%loopTime;
  unsigned long currentStep = timer%fadeTime;

  int activeLED = timer/fadeTime;
  unsigned char brightness = currentStep*maxBrightness/fadeTime;

  ShiftPWM.SetAll(0);
  ShiftPWM.SetOne((activeLED+1)%numOutputs,brightness);
  ShiftPWM.SetOne(activeLED,maxBrightness-brightness);
}

void inOutAll(void){  // Fade in all outputs
  unsigned char brightness;
  unsigned long fadeTime = 2000;
  unsigned long time = millis()-startTime;
  unsigned long currentStep = time%(fadeTime*2);

  if(currentStep <= fadeTime ){
    brightness = currentStep*maxBrightness/fadeTime; ///fading in
  }
  else{
    brightness = maxBrightness-(currentStep-fadeTime)*maxBrightness/fadeTime; ///fading out;
  }
  ShiftPWM.SetAll(brightness);
}

void alternatingColors(void){ // Alternate LED's in 6 different colors
  unsigned long holdTime = 2000;
  unsigned long time = millis()-startTime;
  unsigned long shift = (time/holdTime)%6;
  for(unsigned int led=0; led<numRGBLeds; led++){
    switch((led+shift)%6){
    case 0:
      ShiftPWM.SetRGB(led,255,0,0);    // red
      break;
    case 1:
      ShiftPWM.SetRGB(led,0,255,0);    // green
      break;
    case 2:
      ShiftPWM.SetRGB(led,0,0,255);    // blue
      break;
    case 3:
      ShiftPWM.SetRGB(led,255,128,0);  // orange
      break;
    case 4:
      ShiftPWM.SetRGB(led,0,255,255);  // turqoise
      break;
    case 5:
      ShiftPWM.SetRGB(led,255,0,255);  // purple
      break;
    }
  }
}

void hueShiftAll(void){  // Hue shift all LED's
  unsigned long cycleTime = 10000;
  unsigned long time = millis()-startTime;
  unsigned long hue = (360*time/cycleTime)%360;
  ShiftPWM.SetAllHSV(hue, 255, 255); 
}

void randomColors(void){  // Update random LED to random color. Funky!
  unsigned long updateDelay = 100;
  static unsigned long previousUpdateTime;
  if(millis()-previousUpdateTime > updateDelay){
    previousUpdateTime = millis();
    ShiftPWM.SetHSV(random(numRGBLeds),random(360),255,255);
  }
}

void rgbLedRainbow(unsigned long cycleTime, int rainbowWidth){
  // Displays a rainbow spread over a few LED's (numRGBLeds), which shifts in hue. 
  // The rainbow can be wider then the real number of LED's.
  unsigned long time = millis()-startTime;
  unsigned long colorShift = (360*time/cycleTime)%360; // this color shift is like the hue slider in Photoshop.

  for(unsigned int led=0;led<numRGBLeds;led++){ // loop over all LED's
    int hue = ((led)*360/(rainbowWidth-1)+colorShift)%360; // Set hue from 0 to 360 from first to last led and shift the hue
    ShiftPWM.SetHSV(led, hue, 255, 255); // write the HSV values, with saturation and value at maximum
  }
}
