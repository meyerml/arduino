#include "Arduino.h"        //mp3
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Stepper.h"
#include <AccelStepper.h>
// Motor pin definitions:
#define motorPin1  31      // IN1 on the ULN2003 driver
#define motorPin2  33     // IN2 on the ULN2003 driver
#define motorPin3  35     // IN3 on the ULN2003 driver
#define motorPin4  37     // IN4 on the ULN2003 driver
// Define the AccelStepper interface type; 4 wire motor in half step mode:
#define MotorInterfaceType 8

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper library with 28BYJ-48 stepper motor:
AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);  
//stepper


//stepper
                          //mp3
 int DELAY = 675;
  int val = 0;
  int rando = 0;
// the setup function runs once when you press reset or power the board
// defines linear motor pins numbers
const int stepPin = 3; 
const int dirPin = 4; 
const int sleepPin = 5;
const int inPin = 2;  
const int ButtonPin = 22;
const int LEDPin1 = 6;
const int LEDPin2 = 7;
const int HiPin = 13;
// next, declare the stepper
// and connect pins 8,9,10,11 to IN1,IN2,IN3,IN4 on ULN2003 board


 // let's create a boolean variable to save the direction of our rotation

void setup() {
    digitalWrite(sleepPin, LOW);
    // Set the maximum steps per second:
  stepper.setMaxSpeed(1000);

    // Sets the two linear motor pins as Outputs

  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(sleepPin,OUTPUT);

      //
  pinMode(HiPin, OUTPUT);
  digitalWrite(HiPin,HIGH);
  pinMode(inPin, INPUT);
  pinMode(ButtonPin, INPUT);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LEDPin1, OUTPUT);
  //stepper
//stepper
  //mp3
    mySoftwareSerial.begin(9600);
  Serial.begin(115200);
delay(200);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(22);  //Set volume value. From 0 to 30
}
  //mp3


// the loop function runs over and over again forever
void loop() {
  //stepper idle
  digitalWrite(31,LOW);
  digitalWrite(33,LOW);
  digitalWrite(35,LOW);
  digitalWrite(37,LOW);
  //stepper idle

  if (digitalRead(ButtonPin) == HIGH){
    //random number

  stepper.setCurrentPosition(0);
  // Run the motor forward at 500 steps/second until the motor reaches 4096 steps (1 revolution):
  while (stepper.currentPosition() != 64) {
    stepper.setSpeed(500);
    stepper.runSpeed();
  }


  }
  val = digitalRead(inPin);
  delay(20);
  Serial.println(val);
//################################################################################
//################################################################################
  if (val==HIGH){
  
    analogWrite(LEDPin2, 175);   // turn the LED on (HIGH is the voltage level)
    analogWrite(LEDPin1, 175);   // turn the LED on (HIGH is the voltage level)




  digitalWrite(31,LOW);
  digitalWrite(33,LOW);
  digitalWrite(35,LOW);
  digitalWrite(37,LOW);
  delay(1000);

  
  
     
   
    //turn eyes off
    digitalWrite(LEDPin2, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(LEDPin1, LOW);    // turn the LED off by making the voltage LOW




  }
  
}

  
