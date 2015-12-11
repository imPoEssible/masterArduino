
#include <Wire.h>
#include <Adafruit_MotorShield.h>
//MOTOR SETUP:
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *M1 = AFMS.getMotor(1);
Adafruit_DCMotor *M2 = AFMS.getMotor(2);

int M1speed = 40; //set motor speeds
int M2speed = 40;

//LED SETUP
// You can choose the latch pin yourself.
const int ShiftPWM_latchPin=8;
const bool ShiftPWM_invertOutputs = false;
const bool ShiftPWM_balanceLoad = false;

#include <ShiftPWM.h>   // include ShiftPWM.h after setting the pins!

// Function prototypes (telling the compiler these functions exist).
void oneByOne(void);
void inOutTwoLeds(void);
void inOutAll(void);
void alternatingColors(void);
void hueShiftAll(void);
void randomColors(void);
void fakeVuMeter(void);
void rgbLedRainbow(unsigned long cycleTime, int rainbowWidth);
void printInstructions(void);

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
byte photoDiode_pin = A5;
int brightness;
long prevTime = 0;
long offsetTime = 0; //Used to create offset

// set up mode variable
byte mode = 0;
byte prev_mode = 0;

void setup() {
  AFMS.begin();
  M1->setSpeed(M1speed);
  M2->setSpeed(M2speed);

  // Sets the number of 8-bit registers that are used.
  ShiftPWM.SetAmountOfRegisters(numRegisters);

  // SetPinGrouping allows flexibility in LED setup. 
  // If your LED's are connected like this: RRRRGGGGBBBBRRRRGGGGBBBB, use SetPinGrouping(4).
  ShiftPWM.SetPinGrouping(8);
  ShiftPWM.Start(pwmFrequency,maxBrightness);
  
  Serial.begin(9600);
  
}

void loop() {
  if (Serial.available() > 0){
    mode = Serial.read() - 48;
  }
//  if (mode == prev_mode){
//    continue;
//  }
  switch(mode){
    case 1:
      serialFlush();
      offsetTime = millis();
      while (millis() - offsetTime<10){
        M1->run(BACKWARD);
        offsetTime=0; //Reseting it
      }
      M1->run(BACKWARD);
      M2->run(BACKWARD);
      rgbLedRainbow(3000,numRGBLeds);
      break;
    case 2:
      serialFlush();
      alternatingColors();
      resetMotor();      
      
    default:
      if ((millis() - prevTime) > 1000){
        brightness = map(analogRead(photoDiode_pin), 500, 900, 0, 255);
        prevTime = millis();
      }
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

void resetMotor(){
  //reset motors
  M1->run(RELEASE);
  M2->run(RELEASE);
}

/*LED PWM FUNCTIONS BELOW*/

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

void fakeVuMeter(void){ // imitate a VU meter
  static unsigned int peak = 0;
  static unsigned int prevPeak = 0;
  static unsigned long currentLevel = 0;
  static unsigned long fadeStartTime = startTime;
  
  unsigned long fadeTime = (currentLevel*2);// go slower near the top

  unsigned long time = millis()-fadeStartTime;
  currentLevel = time%(fadeTime);

  if(currentLevel==peak){
    // get a new peak value
    prevPeak = peak;
    while(abs(peak-prevPeak)<5){
      peak =  random(numRGBLeds); // pick a new peak value that differs at least 5 from previous peak
    }
  }

  if(millis() - fadeStartTime > fadeTime){
    fadeStartTime = millis();
    if(currentLevel<peak){ //fading in
      currentLevel++;
    }
    else{ //fading out
      currentLevel--;
    }
  }
  // animate to new top
  for(unsigned int led=0;led<numRGBLeds;led++){
    if(led<currentLevel){
      int hue = (numRGBLeds-1-led)*120/numRGBLeds; // From green to red
      ShiftPWM.SetHSV(led,hue,255,255); 
    }
    else if(led==currentLevel){
      int hue = (numRGBLeds-1-led)*120/numRGBLeds; // From green to red
      int value;
      if(currentLevel<peak){ //fading in        
        value = time*255/fadeTime;
      }
      else{ //fading out
        value = 255-time*255/fadeTime;
      }
      ShiftPWM.SetHSV(led,hue,255,value);       
    }
    else{
      ShiftPWM.SetRGB(led,0,0,0);
    }
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
