/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#define ONBOARD_LED 2
#define buttonPin 22
#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>





// REPLACE WITH THE MAC Address of your receiver 84:CC:A8:A3:00:9C

uint8_t broadcastAddress[] = {0x84, 0xCC, 0xA8, 0xA3, 0x00, 0x9C};

// Define variables to store BME280 readings to be sent
bool alarm_aus;
bool stoppstatus=1;


// Define variables to store incoming readings
bool alarm_in;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    char a[32];
    bool alarm;
    bool stopp;
} struct_message;

// Create a struct_message called BME280Readings to hold sensor readings
struct_message outgoingReadings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  
  alarm_in = incomingReadings.alarm;
  Serial.print("Der Button/Alarm ist ");
  Serial.println(alarm_in);
}
 
void setup() {
  pinMode(ONBOARD_LED,OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);
  // Init Serial Monitor
  Serial.begin(115200);

 
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
 delay(20);
 //Serial.print(digitalRead(buttonPin));
  // Set values to send
  if(digitalRead(buttonPin)!=stoppstatus){
    stoppstatus=!stoppstatus;  //stoppstatus 0 beduetet stopp
      outgoingReadings.stopp=!stoppstatus;


  // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingReadings, sizeof(outgoingReadings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
if (alarm_in==1){
  digitalWrite(ONBOARD_LED,HIGH);
} else {
  digitalWrite(ONBOARD_LED,LOW);
}

}
