#include "Arduino.h"        //mp3
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Stepper.h"
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);  
//stepper
       // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
// initialize the stepper library on pins 8 through 11:
const int stepsPerRevolution = 2048;

Stepper myStepper(stepsPerRevolution, 14, 16, 15, 17);
//stepper
                          //mp3
 int DELAY = 675;
  int val = 0;
// the setup function runs once when you press reset or power the board
// defines linear motor pins numbers
const int stepPin = 3; 
const int dirPin = 4; 
const int sleepPin = 5;
const int inPin = 2;                                      //Pin 2 soll der input pin sein.
const int LEDPin1 = 7;
const int LEDPin2 = 6;
int rounds = 0;
// next, declare the stepper
// and connect pins 8,9,10,11 to IN1,IN2,IN3,IN4 on ULN2003 board


 // let's create a boolean variable to save the direction of our rotation

void setup() {
  myStepper.setSpeed(10);
    // Sets the two linear motor pins as Outputs

  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(sleepPin,OUTPUT);
  digitalWrite(sleepPin, LOW);
      //
  pinMode(inPin, INPUT);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LEDPin1, OUTPUT);
  //stepper
//stepper
  //mp3
    mySoftwareSerial.begin(9600);
  Serial.begin(115200);

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
  digitalWrite(14,LOW);
  digitalWrite(15,LOW);
  digitalWrite(16,LOW);
  digitalWrite(17,LOW);
  //stepper idle
  val = digitalRead(2);
 //Serial.println(val);
//################################################################################
//################################################################################
  if (val==HIGH){
    //random number


    digitalWrite(LEDPin2, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(LEDPin1, HIGH);   // turn the LED on (HIGH is the voltage level)
    Serial.println(val);
  }
else if (val==LOW){
  digitalWrite(LEDPin2, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(LEDPin1, LOW);   // turn the LED on (HIGH is the voltage level)
}








delay(20);
  }
