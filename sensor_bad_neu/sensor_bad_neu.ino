//www.elegoo.com
//2018.10.24
#include <math.h>
#include <Wire.h>

#include "TimeLord.h"
#include <ds3231.h>
struct ts t;
const int PROGMEM TIMEZONE = 1;
const double PROGMEM LONGITUDE = 7.75;
const double PROGMEM LATITUDE = 49.42;

TimeLord myLord;

int ledPin = 7;  // LED on Pin 13 of Arduino
int pirPin1 = 6; // Input for HC-S501
int pirPin2 = 5; // Input for HC-S501
int pirValue1; // Place to store read PIR Value
int pirValue2; // Place to store read PIR Value
int pirValue;

void setup()
{

  Serial.begin(9600);
    Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  /*----------------------------------------------------------------------------
  In order to synchronise your clock module, insert timetable values below !
  ----------------------------------------------------------------------------*/
  t.hour=15; 
  t.min=19;
  t.sec=0;
  t.mday=20;
  t.mon=3;
  t.year=2021;
 
  //DS3231_set(t);
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin1, INPUT);
  pinMode(pirPin2, INPUT);
  digitalWrite(ledPin, HIGH);
  Serial.begin(9600);



  myLord.TimeZone(TIMEZONE * 60);
  myLord.Position(LATITUDE, LONGITUDE);
  myLord.DstRules(3,2,11,1, 60); // second sunday in march thru first


  
}


void loop()
{
  //dt = clock.getDateTime();
  
//  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
//  &year);
 /*
  Serial.print("Raw data: ");
  Serial.print(dt.year);   Serial.print("-");
  Serial.print(dt.month);  Serial.print("-");
  Serial.print(dt.day);    Serial.print(" ");
  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");
*/

  DS3231_get(&t);
  Serial.print("Date : ");
  Serial.print(t.mday);
  Serial.print("/");
  Serial.print(t.mon);
  Serial.print("/");
  Serial.print(t.year);
  Serial.print("\t Hour : ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(".");
  Serial.println(t.sec);
 
  delay(1000);
 // Serial.print(dayOfMonth);
 
   byte day[] = {0, 0, 0, t.mday, t.mon, t.year};

     myLord.SunRise(day);
long rise_h = day[tl_hour];
long rise_m = day[tl_minute];
long rise_s = rise_h*3600 + rise_m*60;

     myLord.SunSet(day);
long set_h = day[tl_hour];
long set_m = day[tl_minute];
long set_s = set_h*3600 + set_m*60;

long now_h = t.hour;
long now_m = t.min;
long now_s = now_h*3600+now_m*60;
Serial.println("rise");
Serial.print(rise_s);
Serial.println("set");
Serial.print(set_s);
Serial.println("now");
Serial.print(now_s);
Serial.println("hour");
Serial.print(now_h);
Serial.println("minute");
Serial.print(now_m);

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
