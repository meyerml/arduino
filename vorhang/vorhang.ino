//#include <esp_now.h>
#include <EEPROM.h>
//#include <WiFi.h>
//#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <espnow.h>


#include <Wire.h>
bool vorhang;
typedef struct struct_message {
    char a[32];
    bool alarm;
    bool stopp;
} struct_message;     
struct_message incomingReadings;
// define the number of bytes you want to access
#define EEPROM_SIZE 1
// Define pin connections & motor's steps per revolution
const int dirPin = 12;
const int stepPin = 13;
const int enPin = 15;
const int switchPin = 5;
const int stepsPerRevolution = 4000;
bool oldstate; // der soll im flash gespeichert werden, HIGH = links, vorhang auf; LOW = rechts, vorhang zu
bool newstate;
bool change;
bool alarm_in;
//Structure example to send data
//Must match the receiver structure


// Create a struct_message called BME280Readings to hold sensor readings
struct_message outgoingReadings;

// Create a struct_message to hold incoming sensor readings
//struct_message incomingReadings;
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("x: ");
  Serial.println(incomingReadings.alarm);
alarm_in = incomingReadings.alarm;
  Serial.println();
    Serial.print("Der Button/Alarm ist ");
  Serial.println(alarm_in);
}
void setup()
{
  
  Serial.begin(9600);
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  EEPROM.begin(EEPROM_SIZE);
  oldstate = EEPROM.read(0);
  Serial.println("oldstate read");

    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  oldstate=digitalRead(switchPin);
}
/*void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);

  alarm_in = incomingReadings.alarm;
  Serial.print("Der Button/Alarm ist ");
  Serial.println(alarm_in);
}*/
void movemotor() {
  Serial.println("vorhang:");
  Serial.println(vorhang);
  if (vorhang == 0) {
    digitalWrite(dirPin, HIGH);
    digitalWrite(enPin, HIGH);

    // Spin motor slowly
    for (int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(stepPin, HIGH);
      delay(2);
      digitalWrite(stepPin, LOW);
      delay(2);
    }
    digitalWrite(enPin, LOW);
  }
  else if (vorhang == 1) {
    digitalWrite(dirPin, LOW);
    digitalWrite(enPin, HIGH);
    // Spin motor slowly
    for (int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(stepPin, HIGH);
      delay(2);
      digitalWrite(stepPin, LOW);
      delay(2);
    }
    digitalWrite(enPin, LOW);


  }
  Serial.println("State changed");
  // save the LED state in flash memory
  EEPROM.write(0, vorhang);
  EEPROM.commit();
  Serial.println("State saved in flash memory");
  //oldstate = newstate;
}
void loop()
{
  Serial.println(oldstate);
  newstate = digitalRead(switchPin);
  change = ! (newstate == oldstate);
  if (change == 1) {
    movemotor();
vorhang=!vorhang;
oldstate=newstate;
  }
  // Set motor direction clockwise
  
  if (alarm_in == 1) {
    if (vorhang == 0) {
      
movemotor();
vorhang=!vorhang;
    }

    alarm_in = 0;
  }
}
