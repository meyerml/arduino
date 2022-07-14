//www.elegoo.com
//2018.10.24
#include <math.h>
#include <Wire.h>
#include <DS3231.h>
#include "TimeLord.h"


const int PROGMEM TIMEZONE = 1;
const double PROGMEM LONGITUDE = 7.75;
const double PROGMEM LATITUDE = 49.42;

TimeLord myLord;
DS3231 clock;
RTCDateTime dt;


int ledPin = 7;  // LED on Pin 13 of Arduino
int pirPin1 = 6; // Input for HC-S501
int pirPin2 = 5; // Input for HC-S501
int pirValue1; // Place to store read PIR Value
int pirValue2; // Place to store read PIR Value
int pirValue;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin1, INPUT);
  pinMode(pirPin2, INPUT);
  digitalWrite(ledPin, HIGH);
  Serial.begin(9600);

  Serial.println("Initialize RTC module");
  // Initialize DS3231
  clock.begin();

  
  // Manual (YYYY, MM, DD, HH, II, SS
  // clock.setDateTime(2016, 12, 9, 11, 46, 00);
  
  // Send sketch compiling time to Arduino
  clock.setDateTime(__DATE__, __TIME__);    
  /*
  Tips:This command will be executed every time when Arduino restarts. 
       Comment this line out to store the memory of DS3231 module
  */
  myLord.TimeZone(TIMEZONE * 60);
  myLord.Position(LATITUDE, LONGITUDE);
  myLord.DstRules(3,2,11,1, 60); // second sunday in march thru first


  
}

void loop()
{
  dt = clock.getDateTime();
  
  // For leading zero look to DS3231_dateformat example

  Serial.print("Raw data: ");
  Serial.print(dt.year);   Serial.print("-");
  Serial.print(dt.month);  Serial.print("-");
  Serial.print(dt.day);    Serial.print(" ");
  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");


   byte day[] = {0, 0, 0, dt.day, dt.month, dt.year};

     myLord.SunRise(day);
long rise_h = day[tl_hour];
long rise_m = day[tl_minute];
long rise_s = rise_h*3600 + rise_m*60;

     myLord.SunSet(day);
long set_h = day[tl_hour];
long set_m = day[tl_minute];
long set_s = set_h*3600 + set_m*60;

long now_h = dt.hour;
long now_m = dt.minute;
long now_s = now_h*3600+now_m*60;

  


  if (now_s >= set_s || now_s <= rise_s ){
    //enable motion sensor

      pirValue1 = digitalRead(pirPin1);
      pirValue2 = digitalRead(pirPin2);
      pirValue = pirValue1 || pirValue2;
      digitalWrite(ledPin, !pirValue);
      Serial.println(pirValue);
  }
  else{
      Serial.println("off");

    digitalWrite(ledPin, HIGH);
  }
  




}
