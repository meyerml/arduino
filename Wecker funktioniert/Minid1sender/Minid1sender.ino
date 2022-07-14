/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#define alarmPin 12 //D6
#define stopPin 13 //D7
#include <ESP8266WiFi.h>
#include <espnow.h>


// REPLACE WITH THE MAC Address of your receiver AC:67:B2:0C:A5:AC
uint8_t broadcastAddress[] = {0xAC, 0x67, 0xB2, 0x0C, 0xA5, 0xAC};
bool stoppstatus = 1;
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  bool alarm;
  bool stopp;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer
bool alarmstatus = 1;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);

  Serial.print("StopPin: ");
  Serial.println(myData.stopp);
  Serial.println();
  stoppstatus = !myData.stopp;
  
}
// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  pinMode(alarmPin,INPUT_PULLUP);
  pinMode(stopPin,OUTPUT);
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  delay(20);
  //Serial.print(alarmstatus);
  if (digitalRead(alarmPin)!=alarmstatus) { // alarmstatus 1: alarm ist aus
    alarmstatus=!alarmstatus;
    // Set values to send
    strcpy(myData.a, "THIS IS A CHAR");
    myData.alarm = !alarmstatus;

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

   
  }
  digitalWrite(stopPin,stoppstatus);
}
