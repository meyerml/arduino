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
int pirPin = 6; // Input for HC-S501
int pirPin2 = 5;
  
int pirValue; // Place to store read PIR Value
int pirValue2;
int pirbool;
long rise_h;
long rise_m;
long rise_s;


long set_h;
long set_m;
long set_s;

long now_h;
long now_m;
long now_s;


byte day[] ={0, 0, 0, 0, 0, 0};
void setup()
{
  Serial.begin(9600);

  Serial.println("Initialize RTC module");
  // Initialize DS3231
  clock.begin();

  
  // Manual (YYYY, MM, DD, HH, II, SS
  // clock.setDateTime(2016, 12, 9, 11, 46, 00);
  
  // Send sketch compiling time to Arduino
  //clock.setDateTime(__DATE__, __TIME__);    
  /*
  Tips:This command will be executed every time when Arduino restarts. 
       Comment this line out to store the memory of DS3231 module
  */
  myLord.TimeZone(TIMEZONE * 60);
  myLord.Position(LATITUDE, LONGITUDE);
  myLord.DstRules(3,2,11,1, 60); // second sunday in march thru first

  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(pirPin2, INPUT);
  digitalWrite(ledPin, LOW);
  
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


day[3] =  dt.day;
day[4] =  dt.month;
day[5] =  dt.year;


     myLord.SunRise(day);
rise_h = day[tl_hour];
rise_m = day[tl_minute];
rise_s = rise_h*3600 + rise_m*60;

     myLord.SunSet(day);
set_h = day[tl_hour];
set_m = day[tl_minute];
set_s = set_h*3600 + set_m*60;

now_h = dt.hour;
now_m = dt.minute;
now_s = now_h*3600+now_m*60;


  //if (1){
  if (now_s >= set_s || now_s <= rise_s ){
    //enable motion sensor
     // Serial.println("LOOP");
      pirValue = digitalRead(pirPin);
      Serial.println("links,rechts");
      Serial.println(pirValue);
      Serial.println(pirValue2);
      pirValue2 = digitalRead(pirPin);
      pirbool = pirValue|pirValue2;
      digitalWrite(ledPin, !pirbool);
      if(pirbool==1){
      Serial.println(pirbool);
      }
  }
  else{

    digitalWrite(ledPin, HIGH);
  }
  

//debugging

Serial.print("now_s ="); Serial.println(now_s);
Serial.print("rise_s ="); Serial.println(rise_s);
Serial.print("set_s ="); Serial.println(set_s);
Serial.print("pirbool ="); Serial.println(pirbool);



}
